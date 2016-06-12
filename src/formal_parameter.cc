// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/formal_parameter.h"

#include <algorithm>
#include <set>

namespace ahoy {

FormalParameter::FormalParameter(const std::set<std::string>& short_forms,
                                 const std::set<std::string>& long_forms,
                                 const std::string& description,
                                 const bool is_flag) :
        short_forms_(short_forms),
        long_forms_(long_forms),
        description_(description),
        required_(!is_flag),
        default_value_(""),
        is_flag_(is_flag) {}

FormalParameter::FormalParameter(const std::set<std::string>& short_forms,
                                 const std::set<std::string>& long_forms,
                                 const std::string& description,
                                 char const * const default_value) :
        FormalParameter(short_forms, long_forms, description, std::string(default_value)) {}

FormalParameter::FormalParameter(const std::set<std::string>& short_forms,
                                 const std::set<std::string>& long_forms,
                                 const std::string& description,
                                 const std::string& default_value) :
        short_forms_(short_forms),
        long_forms_(long_forms),
        description_(description),
        required_(false),
        default_value_(default_value),
        is_flag_(false) {}

FormalParameter::~FormalParameter() {}

bool FormalParameter::HasShortForm(const std::string& form) const {
    return short_forms_.find(form) != short_forms_.end();
}

bool FormalParameter::HasLongForm(const std::string& form) const {
    return long_forms_.find(form) != long_forms_.end();
}

bool FormalParameter::IsFlag() const {
    return is_flag_;
}

const std::set<std::string>& FormalParameter::short_forms() const {
    return short_forms_;
}

const std::set<std::string>& FormalParameter::long_forms() const {
    return long_forms_;
}

const std::string& FormalParameter::description() const {
    return description_;
}

bool FormalParameter::IsRequired() const {
    return required_;
}

const std::string* FormalParameter::default_value() const {
    if (is_flag_ || required_) {
        return nullptr;
    } else {
        return &default_value_;
    }
}

} // namespace ahoy
