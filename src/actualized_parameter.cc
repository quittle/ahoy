#include "ahoy/actualized_parameter.h"

#include <algorithm>
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

long ActualizedParameter::AsLong() const {
    return stol(value_);
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
