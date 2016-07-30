// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/parser.h"

#include <algorithm>
#include <climits>
#include <locale>
#include <sstream>
#include <set>

#include "ahoy/arg_generator.h"
#include "ahoy/arg_size.h"
#include "ahoy/args.h"

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

// Converts returns a lowercase version of the string passed in
std::string ToLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    return value;
}

// The following are implementations of how to convert strings and booleans to various types.
// For booleans, strings checked if they match one of several affirmation strings.
// For chars, booleans are converted to 1 and 0 and strings are expected to be a single character
// long.
// For numeric types, booleans are converted to 1 and 0 and strings are parsed to numbers

bool AssignBool(bool * const pointer, const std::string& value) {
    const std::set<std::string> trues = { "true", "t", "yes", "y", "on", "1" };
    *pointer = trues.find(ToLower(value)) != trues.end();
    return true;
}

bool AssignBool(bool * const pointer, const bool value) {
    *pointer = value;
    return true;
}

bool AssignChar(char * const pointer, const std::string& value) {
    if (value.size() != 1) {
        return false;
    }

    *pointer = value[0];
    return true;
}

bool AssignChar(char * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignUChar(unsigned char * const pointer, const std::string& value) {
    if (value.size() != 1) {
        return false;
    }

    *pointer = value[0];
    return true;
}

bool AssignUChar(unsigned char * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignShort(short * const pointer, const std::string& value) {
    try {
        const int int_value = std::stoi(value);
        if (int_value > SHRT_MAX || int_value < SHRT_MIN) {
            return false;
        }
        *pointer = int_value;
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignShort(short * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignUShort(unsigned short * const pointer, const std::string& value) {
    try {
        const unsigned long ulong_value = std::stoul(value);
        if (ulong_value > USHRT_MAX) {
            return false;
        }
        *pointer = ulong_value;
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignUShort(unsigned short * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignInt(int * const pointer, const std::string& value) {
    try {
        *pointer = std::stoi(value);
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignInt(int * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignUInt(uint * const pointer, const std::string& value) {
    try {
        const unsigned long ulong_value = std::stoul(value);
        if (ulong_value > UINT_MAX) {
            return false;
        }
        *pointer = ulong_value;
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignUInt(uint * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignLong(long * const pointer, const std::string& value) {
    try {
        *pointer = std::stol(value);
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignLong(long * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignULong(unsigned long * const pointer, const std::string& value) {
    try {
        *pointer = std::stoul(value);
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignULong(unsigned long * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignLongLong(long long * const pointer, const std::string& value) {
    try {
        *pointer = std::stoll(value);
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignLongLong(long long * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignULongLong(unsigned long long * const pointer, const std::string& value) {
    try {
        *pointer = std::stoull(value);
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignULongLong(unsigned long long * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignFloat(float * const pointer, const std::string& value) {
    try {
        *pointer = std::stof(value);
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignFloat(float * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignDouble(double * const pointer, const std::string& value) {
    try {
        *pointer = std::stod(value);
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignDouble(double * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignLongDouble(long double * const pointer, const std::string& value) {
    try {
        *pointer = std::stold(value);
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignLongDouble(long double * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignString(std::string * const pointer, const std::string& value) {
    *pointer = value;
    return true;
}

bool AssignString(std::string * const pointer, const bool value) {
    *pointer = value ? "true" : "false";
    return true;
}

// Maps a Type to an assignment method and returns the success of the assignment
template<typename T>
bool Assign(void * const pointer, const ahoy::internal::Type type, const T& value) {
    switch (type) {
        case ahoy::internal::Type::BOOL:
            return AssignBool((bool*) pointer, value);
        case ahoy::internal::Type::CHAR:
            return AssignChar((char*) pointer, value);
        case ahoy::internal::Type::U_CHAR:
            return AssignUChar((unsigned char*) pointer, value);
        case ahoy::internal::Type::SHORT:
            return AssignShort((short*) pointer, value);
        case ahoy::internal::Type::U_SHORT:
            return AssignUShort((unsigned short*) pointer, value);
        case ahoy::internal::Type::INT:
            return AssignInt((int*) pointer, value);
        case ahoy::internal::Type::U_INT:
            return AssignUInt((unsigned int*) pointer, value);
        case ahoy::internal::Type::LONG:
            return AssignLong((long*) pointer, value);
        case ahoy::internal::Type::U_LONG:
            return AssignULong((unsigned long*) pointer, value);
        case ahoy::internal::Type::LONG_LONG:
            return AssignLongLong((long long*) pointer, value);
        case ahoy::internal::Type::U_LONG_LONG:
            return AssignULongLong((unsigned long long*) pointer, value);
        case ahoy::internal::Type::FLOAT:
            return AssignFloat((float*) pointer, value);
        case ahoy::internal::Type::DOUBLE:
            return AssignDouble((double*) pointer, value);
        case ahoy::internal::Type::LONG_DOUBLE:
            return AssignLongDouble((long double*) pointer, value);
        case ahoy::internal::Type::STRING:
            return AssignString((std::string*) pointer, value);
        default:
            return false;
    }
}

} // namespace

namespace ahoy {

Parser::Parser() {}

Parser::~Parser() {}

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
