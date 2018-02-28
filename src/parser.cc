// Copyright (c) 2016, 2018 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/parser.h"

#include <list>
#include <locale>
#include <sstream>
#include <set>
#include <utility>

#include "ahoy/internal/arg_generator.h"
#include "ahoy/internal/arg_size.h"
#include "ahoy/internal/args.h"
#include "ahoy/internal/assign.h"

#include <iostream> // REMOVE ME

namespace {

// Tries to get either the short forms or long forms from |flag_arg| as required by
// |formal_parameter|.  Returns true if succesful, or false if not.
bool GetForms(const ahoy::internal::FlagArg& flag_arg,
              const ahoy::internal::FormalParameter& formal_parameter,
              std::set<std::string>& ret) {
    switch (flag_arg.size()) {
        case ahoy::internal::ArgSize::SHORT:
            ret = formal_parameter.short_forms();
            return true;
        case ahoy::internal::ArgSize::LONG:
            ret = formal_parameter.long_forms();
            return true;
        default:
            return false;
    }
}

} // namespace

namespace ahoy {

Parser::Parser() {}

Parser::~Parser() {}

Parser& Parser::withOptions(const std::vector<Option2>& options) {
    current_options_ = options;
    return *this;
}

Parser& Parser::then(const std::vector<Option2>& options) {
    next_options_ = options;
    return *this;
}

std::string Parser::HelpMessage(const Newline newline) const {
    std::stringstream ss;
    for (void * const pointer : named_pointers_list_) {
        const internal::FormalParameter& param = named_pointers_.find(pointer)->second;

        bool first_form = true;

        // Add short forms
        for (const std::string& form : param.short_forms()) {
            if (first_form) {
                first_form = false;
            } else {
                ss << " ";
            }

            ss << "-" << form;
        }

        // Add long forms
        for (const std::string& form : param.long_forms()) {
            if (first_form) {
                first_form = false;
            } else {
                ss << " ";
            }

            ss << "--" << form;
        }

        if (param.required()) {
            ss << " (Required)";
        }

        if (param.flag()) {
            ss << " (Flag)";
        }

        ss << " [" << param.type() << "]";

        if (!param.name().empty()) {
            ss << " - " << param.name();
        }

        if (!param.description().empty()) {
            ss << " - " << param.description();
        }

        // Add line ending
        ss << newline;
    }

    for (void * const pointer : positional_pointers_list_) {
        const internal::FormalParameter& param = positional_pointers_.find(pointer)->second;

        if (param.required()) {
            ss << "(Required) ";
        }

        ss << "[" << param.type() << "]";

        if (!param.name().empty()) {
            ss << " - " << param.name();
        }

        if (!param.description().empty()) {
            ss << " - " << param.description();
        }

        ss << newline;
    }
    return ss.str();
}

bool Parser::Parse2(const int argc, char const * const argv[]) const {
    std::list<std::string> args(argv + 1, argv + argc);
    Option2 root((bool*) nullptr);
    root.must_consume_ = true;
    root.withOptions(current_options_).then(next_options_);
    root.consume(args);
    return args.size() == 0;
}

bool Parser::Parse(const int argc, char const * const argv[]) const {
    internal::ArgGenerator arg_generator(argc, argv);

    // Track the next positional parameter
    auto cur_positional_pointer = positional_pointers_list_.begin();
    // Track the required parameters found
    std::set<void*> required_params;

    // Update all the flags to false first
    for (const auto& entry : named_pointers_) {
        void * const pointer = entry.first;
        const internal::FormalParameter& formal_parameter = entry.second;

        if (formal_parameter.flag()) {
            if (!Assign(pointer, formal_parameter.type(), false)) {
                return false;
            }
        }
    }

    while (arg_generator.Available()) {
        const internal::ArgGenerator::Result arg_generator_result = arg_generator.Peek();

        // Check if there is potential for a flag or named argument
        const internal::FlagArg flag_arg = arg_generator_result.flag_arg;
        const internal::NamedArg named_arg = arg_generator_result.named_arg;
        const bool is_flag_arg_valid = flag_arg != internal::FlagArg::INVALID;
        const bool is_named_arg_valid = named_arg != internal::NamedArg::INVALID;

        bool found = false;
        if (is_flag_arg_valid || is_named_arg_valid) {
            const std::string& flag_arg_name = flag_arg.name();
            const std::string& named_arg_name = named_arg.name();
            for (const auto& entry : named_pointers_) {
                void * const pointer = entry.first;
                const internal::FormalParameter& formal_parameter = entry.second;

                // Reused in both cases
                std::set<std::string> forms;
                if (formal_parameter.flag() && is_flag_arg_valid && // Is flag case
                        GetForms(flag_arg, formal_parameter, forms) && // Has valid size
                        forms.find(flag_arg_name) != forms.end()) { // Matches its forms

                    // Assignment may fail if type mis-match occurs (e.g. user provides "hello" but
                    // the type is int).
                    if (!Assign(pointer, formal_parameter.type(), true)) {
                        return false;
                    }

                    // Note that a required param was found
                    if (formal_parameter.required()) {
                        required_params.insert(pointer);
                    }
                    arg_generator.AdvanceFlagArg();
                    found = true;
                    break;
                } else if (!formal_parameter.flag() &&
                        is_named_arg_valid &&
                        GetForms(named_arg, formal_parameter, forms) &&
                        forms.find(named_arg_name) != forms.end()) {
                    if (!Assign(entry.first, formal_parameter.type(), named_arg.value())) {
                        return false;
                    }
                    if (formal_parameter.required()) {
                        required_params.insert(pointer);
                    }
                    arg_generator.AdvanceNamedArg();
                    found = true;
                    break;
                }
            }
        }

        // Occurs when the current arg cannot be a flag or regular named arg
        if (!found) {
            const internal::PositionalArg positional_arg = arg_generator_result.positional_arg;
            if (positional_arg == internal::PositionalArg::INVALID ||
                    cur_positional_pointer == positional_pointers_list_.end()) {
                return false;
            }
            const internal::FormalParameter& formal_parameter =
                    positional_pointers_.find(*cur_positional_pointer)->second;
            if (!Assign(*cur_positional_pointer, formal_parameter.type(), positional_arg.value())) {
                return false;
            }
            if (formal_parameter.required()) {
                required_params.insert(*cur_positional_pointer);
            }
            arg_generator.AdvancePositionalArg();
            cur_positional_pointer++;
        }
    }

    size_t n_required_params = 0;
    // Check for required parameters
    for (const auto& entry : named_pointers_) {
        if (entry.second.required()) {
            n_required_params++;
        }
    }
    for (const auto& entry : positional_pointers_) {
        if (entry.second.required()) {
            n_required_params++;
        }
    }

    if (n_required_params != required_params.size()) {
        return false;
    }

    return true;
}

} // namespace ahoy
