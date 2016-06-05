#include "ahoy/param.h"

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


Param::Param() : Param("") {}

Param::Param(char const * const value) : Param(std::string(value)) {}

Param::Param(const std::string& value) : value_(value) {}
Param::Param(const bool value) : value_(value ? "true" : "false") {}

std::string Param::AsString() const {
    return value_;
}

int Param::AsInt() const {
    return stoi(value_);
}

long Param::AsLong() const {
    return stol(value_);
}

bool Param::AsBool() const {
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

bool Param::operator==(const Param& rhs) const {
    return value_ == rhs.value_;
}

bool Param::operator!=(const Param& rhs) const {
    return !(*this == rhs);
}

} // namespace ahoy

std::ostream& operator<<(std::ostream& os, const ahoy::Param& param) {
    os << param.AsString();
    return os;
}
