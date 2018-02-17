// Copyright (c) 2016, 2018 Dustin Doloff
// Licensed under Apache License v2.0

#include <sstream>

#include "ahoy/internal/type.h"

#include <gtest/gtest.h>

namespace {

// Gets the string representation of |type|
std::string TypeToString(const ahoy::internal::Type& type) {
    std::stringstream ss;
    ss << type;
    return ss.str();
}

} // namespace

namespace ahoy {
namespace internal {

TEST(Type, StreamOperator) {
    EXPECT_EQ("Invalid", TypeToString(Type::INVALID));
    EXPECT_EQ("Unsigned Character", TypeToString(Type::U_CHAR));
    EXPECT_EQ("Long", TypeToString(Type::LONG));
    EXPECT_EQ("String", TypeToString(Type::STRING));
}

} // namespace internal
} // namespace ahoy
