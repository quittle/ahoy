// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/formal_parameter.h"

namespace ahoy {
namespace internal {

FormalParameter::FormalParameter() :  required_(false), flag_(false), type_(Type::INVALID) {}
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

const std::set<std::string>& FormalParameter::short_forms() const {
    return short_forms_;
}

void FormalParameter::short_forms(const std::set<std::string>& short_forms) {
    short_forms_ = short_forms;
}

const std::set<std::string>& FormalParameter::long_forms() const {
    return long_forms_;
}

void FormalParameter::long_forms(const std::set<std::string>& long_forms) {
    long_forms_ = long_forms;
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

Type FormalParameter::type() const {
    return type_;
}

void FormalParameter::type(const Type type) {
    type_ = type;
}

} // namespace internal
} // namespace ahoy
