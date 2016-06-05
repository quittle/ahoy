#include "ahoy/arg.h"

#include <algorithm>
#include <set>

namespace ahoy {

Arg::Arg(const std::set<std::string>& short_forms, const std::set<std::string>& long_forms,
            const std::string& description, const bool required, const std::string& default_value) :
        short_forms_(short_forms),
        long_forms_(long_forms),
        description_(description),
        required_(required),
        default_value_(default_value),
        is_flag_(false) {}

Arg::Arg(const std::set<std::string>& short_forms, const std::set<std::string>& long_forms,
            const std::string& description) :
        short_forms_(short_forms),
        long_forms_(long_forms),
        description_(description),
        required_(false),
        default_value_(""),
        is_flag_(true) {}

Arg::~Arg() {}

bool Arg::HasShortForm(const std::string& form) const {
    return short_forms_.find(form) != short_forms_.end();
}

bool Arg::HasLongForm(const std::string& form) const {
    return long_forms_.find(form) != long_forms_.end();
}

bool Arg::IsFlag() const {
    return is_flag_;
}

const std::set<std::string>& Arg::short_forms() const {
    return short_forms_;
}

const std::set<std::string>& Arg::long_forms() const {
    return long_forms_;
}

const std::string& Arg::description() const {
    return description_;
}

bool Arg::required() const {
    return required_;
}

const std::string* Arg::default_value() const {
    if (is_flag_ || required_) {
        return nullptr;
    } else {
        return &default_value_;
    }
}

} // namespace ahoy
