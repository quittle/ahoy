// Copyright (c) 2018 Dustin Toff
// Licensed under Apache License v2.0

#include "ahoy/parameter.h"

#include "ahoy/internal/assign.h"

namespace {

// Attempts to consume some options, returning the new consumed value
ahoy::internal::size_t try_options(const std::vector<std::string>& args,
                                   std::set<const ahoy::Parameter*>& available_options,
                                   const ahoy::internal::size_t start,
                                   ahoy::internal::size_t consumed) {
    bool parsed_something;
    do {
        parsed_something = false;
        for (ahoy::Parameter const * const parameter : available_options) {
            ahoy::internal::size_t consumption = parameter->consume(args, start + consumed);
            if (consumption > 0) {
                available_options.erase(parameter);
                consumed += consumption;
                parsed_something = true;
            }
        }
    } while (parsed_something);

    return consumed;
}

} // namespace

namespace ahoy {

Parameter::~Parameter() {}

Parameter& Parameter::withOptions(const std::vector<Parameter>& parameters) {
    current_options_ = parameters;
    return *this;
}


Parameter& Parameter::then(const std::vector<Parameter>& parameters) {
    next_options_ = parameters;
    return *this;
}

internal::size_t Parameter::consume(const std::vector<std::string>& args, internal::size_t start)
        const {
    if (!is_valid()) {
        return -1;
    };

    const internal::size_t args_available = args.size() - start;

    if (args_available <= 0) {
        return -1;
    }

    internal::size_t consumed = 0;

    // This whole section is where this instance consumes the arguments passed in
    if (fp_.is_positional()) {
        if (fp_.flag()) {
            if (!Assign(storage_, fp_.type(), true)) {
                return -1;
            }
        } else {
            if (!Assign(storage_, fp_.type(), args[start])) {
                return -1;
            }
        }
        consumed = 1;
    } else {
        const std::string& arg = args[start];

        const std::set<std::string>& forms = fp_.forms();
        if (forms.count(arg)) {
            if (fp_.flag()) {
                if (!Assign(storage_, fp_.type(), true)) {
                    return -1;
                }
                consumed = 1;
            } else if (args_available >= 2) {
                if (!Assign(storage_, fp_.type(), args[start + 1])) {
                    return -1;
                }
                consumed = 2;
            }
        } else if (!fp_.flag()) {
            for (const std::string& form : forms) {
                if (arg.find(form + "=") == 0) {
                    if (!Assign(storage_, fp_.type(), arg.substr(form.size() + 1))) {
                        return -1;
                    }
                    consumed = 1;
                    break;
                }
            }
        }
    }

    // Convert to a set
    std::set<const Parameter*> available_options;
    for (const Parameter& parameter : current_options_) {
        available_options.emplace(&parameter);
    }

    // Try options
    consumed = try_options(args, available_options, start, consumed);

    // Find at most one next that works
    for (const Parameter& parameter : next_options_) {
        internal::size_t consumption = parameter.consume(args, start + consumed);
        if (consumption <= 0) {
            if (parameter.fp_.required()) {
                return -1;
            }
        } else {
            consumed += consumption;
            break;
        }
    }

    consumed = try_options(args, available_options, start, consumed);

    for (Parameter const * const parameter : available_options) {
        if (parameter->fp_.required()) {
            return -1;
        }
    }

    return consumed;
}

const std::vector<Parameter>& Parameter::current_options() const {
    return current_options_;
}

const std::vector<Parameter>& Parameter::next_options() const {
    return next_options_;
}

bool Parameter::operator ==(const Parameter& other) const {
    return storage_ == other.storage_ &&
            fp_ == other.fp_ &&
            current_options_ == other.current_options_ &&
            next_options_ == other.next_options_;
}

bool Parameter::operator !=(const Parameter& other) const {
    return !(*this == other);
}

// Currently just validates that next_options_ only contains at most one ahoy::Required()
bool Parameter::is_valid() const {
    bool found_required = false;
    for (const ahoy::Parameter& parameter : next_options_) {
        if (parameter.fp_.required()) {
            if (found_required) {
                return false;
            } else {
                found_required = true;
            }
        }
    }
    return true;
}

void Parameter::BuildFormalNamedParameter(internal::FormalParameter*) {}

} // namespace ahoy
