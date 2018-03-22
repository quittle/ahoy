// Copyright (c) 2016, 2018 Dustin Toff
// Licensed under Apache License v2.0

#include "ahoy/internal/formal_parameter.h"

namespace ahoy {
namespace internal {

FormalParameter::FormalParameter() :  name_(), description_(), forms_(), marker_(), required_(false), flag_(false), type_(Type::INVALID) {}
FormalParameter::~FormalParameter() {}

const std::string& FormalParameter::name() const {
    return name_;
}

void FormalParameter::name(const std::string& name) {
    name_ = name;
}

const std::string& FormalParameter::description() const {
    return description_;
}

void FormalParameter::description(const std::string& description) {
    description_ = description;
}

void FormalParameter::short_forms(const std::set<std::string>& short_forms) {
    for (const std::string& form : short_forms) {
        forms_.emplace("-" + form);
    }
}

void FormalParameter::long_forms(const std::set<std::string>& long_forms) {
    for (const std::string& form : long_forms) {
        forms_.emplace("--" + form);
    }
}

const std::set<std::string>& FormalParameter::forms() const {
    return forms_;
}

const std::string& FormalParameter::marker() const {
    return marker_;
}

void FormalParameter::marker(const std::string& marker) {
    marker_ = marker;
}

bool FormalParameter::required() const {
    return required_;
}

void FormalParameter::required(const bool required) {
    required_ = required;
}

bool FormalParameter::flag() const {
    return flag_;
}

void FormalParameter::flag(const bool flag) {
    flag_ = flag;
}

bool FormalParameter::is_positional() const {
    return forms_.size() == 0;
}

Type FormalParameter::type() const {
    return type_;
}

void FormalParameter::type(const Type type) {
    type_ = type;
}

} // namespace internal
} // namespace ahoy
