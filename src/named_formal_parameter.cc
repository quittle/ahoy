#include "ahoy/named_formal_parameter.h"

namespace ahoy {

NamedFormalParameter::NamedFormalParameter(const std::string& name,
                                           const std::string& description) :
        name_(name),
        description_(description) {}

NamedFormalParameter::~NamedFormalParameter() {}

const std::string& NamedFormalParameter::name() const {
    return name_;
}

const std::string& NamedFormalParameter::description() const {
    return description_;
}

} // namespace ahoy
