#include "ahoy/named_arg.h"

namespace ahoy {

NamedArg::NamedArg(const std::string& name, const std::string& description) :
        name_(name),
        description_(description) {}

NamedArg::~NamedArg() {}

const std::string& NamedArg::name() const {
    return name_;
}

const std::string& NamedArg::description() const {
    return description_;
}

} // namespace ahoy
