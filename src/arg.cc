#include "ahoy/arg.h"

#include <algorithm>
#include <set>

namespace ahoy {

Arg::Arg(const std::set<std::string>& short_forms,
            const std::set<std::string>& long_forms,
            const bool required,
            const std::string& description,
            const std::string& default_value) :
        short_forms_(short_forms),
        long_forms_(long_forms),
        required_(required),
        description_(description),
        default_value_(default_value) {}

Arg::~Arg() {}

const std::set<std::string>& Arg::short_forms() const {
    return short_forms_;
}

const std::set<std::string>& Arg::long_forms() const {
    return long_forms_;
}

bool Arg::required() const {
    return required_;
}

const std::string& Arg::description() const {
    return description_;
}

const std::string& Arg::default_value() const {
    return default_value_;
}

} // namespace ahoy
