// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/actualized_parameter.h"

#include <algorithm>
#include <climits>
#include <set>
#include <stdexcept>

namespace {

const std::set<std::string> kBoolsTrue = {
    "true",
    "t",
    "1",
    "y",
    "yes"
};

const std::set<std::string> kBoolsFalse = {
    "false",
    "f",
    "0",
    "n",
    "no"
};

} // namespace

namespace ahoy {

ActualizedParameter::ActualizedParameter() : ActualizedParameter("") {}

ActualizedParameter::ActualizedParameter(char const * const value) :
        ActualizedParameter(std::string(value)) {}

ActualizedParameter::ActualizedParameter(const std::string& value) : value_(value) {}
ActualizedParameter::ActualizedParameter(const bool value) : value_(value ? "true" : "false") {}

std::string ActualizedParameter::AsString() const {
    return value_;
}

int ActualizedParameter::AsInt() const {
    return stoi(value_);
}

unsigned int ActualizedParameter::AsUInt() const {
    const unsigned long value = stoul(value_);
    if (value > UINT_MAX) {
        throw std::out_of_range(value_ + " is not in unsigned int range");
    }
    return (unsigned int) stol(value_);
}

long ActualizedParameter::AsLong() const {
    return stol(value_);
}

unsigned long ActualizedParameter::AsULong() const {
    return stoul(value_);
}

bool ActualizedParameter::AsBool() const {
    std::string lower = value_;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (kBoolsTrue.find(lower) != kBoolsTrue.end()) {
        return true;
    } else if (kBoolsFalse.find(lower) != kBoolsFalse.end()) {
        return false;
    } else {
        throw std::invalid_argument(value_ + " is not a valid boolean arg");
    }
}

bool ActualizedParameter::operator==(const ActualizedParameter& rhs) const {
    return value_ == rhs.value_;
}

bool ActualizedParameter::operator!=(const ActualizedParameter& rhs) const {
    return !(*this == rhs);
}

} // namespace ahoy

std::ostream& operator<<(std::ostream& os, const ahoy::ActualizedParameter& param) {
    os << param.AsString();
    return os;
}
