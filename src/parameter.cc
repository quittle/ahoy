// Copyright (c) 2018 Dustin Toff
// Licensed under Apache License v2.0

#include "ahoy/parameter.h"

#include "ahoy/internal/assign.h"

namespace {

// Gets the nth element for a list
template<typename T>
const T& get_nth(const std::list<T> list, const std::size_t index) {
    auto it = list.begin();
    std::advance(it, index);
    return *it;
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

bool Parameter::consume(std::list<std::string>& args) const {
    bool consumed(false);

    if (args.size() == 0) {
        return false;
    }

    if (fp_.is_positional() && args.size() >= 1) {
        if (fp_.flag()) {
            if (!Assign(storage_, fp_.type(), true)) {
                return false;
            }
        } else if (!Assign(storage_, fp_.type(), args.front())) {
            return false;
        }
        args.pop_front();
        consumed = true;
    } else {
        const std::string& arg = args.front();
        const std::set<std::string>& forms = fp_.forms();
        if (forms.count(arg)) {
            if (fp_.flag()) {
                if (!Assign(storage_, fp_.type(), true)) {
                    return false;
                }
                args.pop_front();
                consumed = true;
            } else if (args.size() >= 2) {
                if (!Assign(storage_, fp_.type(), get_nth(args, 1))) {
                    return false;
                }
                args.pop_front();
                args.pop_front();
                consumed = true;
            }
        } else if (!fp_.flag()) {
            for (const std::string& form : forms) {
                if (arg.find(form + "=") == 0) {
                    if (!Assign(storage_, fp_.type(), arg.substr(form.size() + 1))) {
                        return false;
                    }
                    args.pop_front();
                    consumed = true;
                    break;
                }
            }
        }
    }

    if (consumed) {
        for (const Parameter& parameter : current_options_) {
            parameter.consume(args);
        }

        if (next_options_.size() == 0) {
            return true;
        }

        for (const Parameter& parameter : next_options_) {
            if (parameter.consume(args)) {
                return true;
            }
        }
    }

    return false;
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

void Parameter::BuildFormalNamedParameter(internal::FormalParameter*) {}

} // namespace ahoy
