// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/args.h"

namespace ahoy {
namespace internal {

// FlagArg

const FlagArg FlagArg::INVALID = FlagArg();

FlagArg::FlagArg() : FlagArg(ArgSize::INVALID, "") {}

FlagArg::FlagArg(const ArgSize& size, const std::string& name) : size_(size), name_(name) {}

FlagArg::~FlagArg() {}


const ArgSize& FlagArg::size() const {
    return size_;
}

void FlagArg::size(const ArgSize size) {
    size_ = size;
}

const std::string& FlagArg::name() const {
    return name_;
}

void FlagArg::name(const std::string& name) {
    name_ = name;
}

bool FlagArg::operator==(const FlagArg& other) const {
    return size_ == other.size_ &&
            name_ == other.name_;
}

bool FlagArg::operator!=(const FlagArg& other) const {
    return !(*this == other);
}

// NamedArg

const NamedArg NamedArg::INVALID = NamedArg();

NamedArg::NamedArg() : NamedArg(ArgSize::INVALID, "", "", false) {}

NamedArg::NamedArg(const ArgSize& size, const std::string& name, const std::string& value,
                   const bool single_part) :
        FlagArg(size, name), value_(value), single_part_(single_part) {}

NamedArg::~NamedArg() {}

const std::string& NamedArg::value() const {
    return value_;
}

void NamedArg::value(const std::string& value) {
    value_ = value;
}

bool NamedArg::single_part() const {
    return single_part_;
}

void NamedArg::single_part(const bool single_part) {
    single_part_ = single_part;
}

bool NamedArg::operator==(const NamedArg& other) const {
    return  ((FlagArg) *this) == (FlagArg) other &&
            value_ == other.value_ &&
            single_part_ == other.single_part_;
}

bool NamedArg::operator!=(const NamedArg& other) const {
    return !(*this == other);
}

// PositionalArg

const PositionalArg PositionalArg::INVALID = PositionalArg();

PositionalArg::PositionalArg() : PositionalArg("") {}

PositionalArg::PositionalArg(const std::string& value) : value_(value) {}

PositionalArg::~PositionalArg() {}

const std::string& PositionalArg::value() const {
    return value_;
}

void PositionalArg::value(const std::string& value) {
    value_ = value;
}

bool PositionalArg::operator==(const PositionalArg& other) const {
    return value_ == other.value_;
}

bool PositionalArg::operator!=(const PositionalArg& other) const {
    return !(*this == other);
}

} // namespace internal
} // namespace ahoy
