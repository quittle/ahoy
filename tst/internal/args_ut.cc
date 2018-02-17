// Copyright (c) 2016, 2018 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/internal/args.h"

#include <gtest/gtest.h>

namespace {

template<typename A, typename B>
void ExpectEQBothWays(const A& a, const B& b) {
    EXPECT_EQ(a, b);
    EXPECT_EQ(b, a);
}

template<typename A, typename B>
void ExpectNEBothWays(const A& a, const B& b) {
    EXPECT_NE(a, b);
    EXPECT_NE(b, a);
}

const ahoy::internal::ArgSize kArgSize = ahoy::internal::ArgSize::SHORT;
const char kName[] = "name";
const char kValue[] = "value";

} // namespace

namespace ahoy {
namespace internal {

TEST(FlagArg, Invalid) {
    EXPECT_EQ(ArgSize::INVALID, FlagArg::INVALID.size());
    EXPECT_EQ("", FlagArg::INVALID.name());
}

TEST(FlagArg, Constructor) {
    const FlagArg arg(kArgSize, kName);
    EXPECT_EQ(kArgSize, arg.size());
    EXPECT_EQ(kName, arg.name());
}

TEST(FlagArg, SetGet) {
    FlagArg arg;

    EXPECT_NE(kName, arg.name());
    arg.name(kName);
    EXPECT_EQ(kName, arg.name());

    EXPECT_NE(kArgSize, arg.size());
    arg.size(kArgSize);
    EXPECT_EQ(kArgSize, arg.size());
}

TEST(FlagArg, Equality) {
    FlagArg arg1;
    FlagArg arg2;

    ExpectEQBothWays(arg1, arg2);

    arg1.name(kName);

    ExpectNEBothWays(arg1, arg2);

    arg2.name(kName);

    ExpectEQBothWays(arg1, arg2);

    arg1.size(kArgSize);

    ExpectNEBothWays(arg1, arg2);

    arg2.size(kArgSize);

    ExpectEQBothWays(arg1, arg2);
}

TEST(NamedArg, Invalid) {
    EXPECT_EQ(ArgSize::INVALID, NamedArg::INVALID.size());
    EXPECT_EQ("", NamedArg::INVALID.name());
    EXPECT_EQ("", NamedArg::INVALID.value());
    EXPECT_FALSE(NamedArg::INVALID.single_part());
}

TEST(NamedArg, Constructor) {
    NamedArg arg(kArgSize, kName, kValue, true);
    EXPECT_EQ(kArgSize, arg.size());
    EXPECT_EQ(kName, arg.name());
    EXPECT_EQ(kValue, arg.value());
    EXPECT_EQ(true, arg.single_part());
}

TEST(NamedArg, SetGet) {
    NamedArg arg;

    EXPECT_NE(kName, arg.name());
    arg.name(kName);
    EXPECT_EQ(kName, arg.name());

    EXPECT_NE(kArgSize, arg.size());
    arg.size(kArgSize);
    EXPECT_EQ(kArgSize, arg.size());

    EXPECT_NE(kValue, arg.value());
    arg.value(kValue);
    EXPECT_EQ(kValue, arg.value());

    EXPECT_FALSE(arg.single_part());
    arg.single_part(true);
    EXPECT_TRUE(arg.single_part());
}

TEST(NamedArg, Equality) {
    NamedArg arg1;
    NamedArg arg2;

    ExpectEQBothWays(arg1, arg2);

    arg1.name(kName);

    ExpectNEBothWays(arg1, arg2);

    arg2.name(kName);

    ExpectEQBothWays(arg1, arg2);

    arg1.size(kArgSize);

    ExpectNEBothWays(arg1, arg2);

    arg2.size(kArgSize);

    ExpectEQBothWays(arg1, arg2);

    arg1.value(kValue);

    ExpectNEBothWays(arg1, arg2);

    arg2.value(kValue);

    ExpectEQBothWays(arg1, arg2);

    arg1.single_part(true);

    ExpectNEBothWays(arg1, arg2);

    arg2.single_part(true);

    ExpectEQBothWays(arg1, arg2);
}

TEST(PositionalArg, Invalid) {
    EXPECT_EQ("", PositionalArg::INVALID.value());
}

TEST(PositionalArg, Constructor) {
    const PositionalArg arg(kValue);
    EXPECT_EQ(kValue, arg.value());
}

TEST(PositionalArg, SetGet) {
    PositionalArg arg;

    EXPECT_NE(kValue, arg.value());
    arg.value(kValue);
    EXPECT_EQ(kValue, arg.value());
}

TEST(PositionalArg, Equality) {
    PositionalArg arg1;
    PositionalArg arg2;

    ExpectEQBothWays(arg1, arg2);

    arg1.value(kValue);

    ExpectNEBothWays(arg1, arg2);

    arg2.value(kValue);

    ExpectEQBothWays(arg1, arg2);
}

} // namespace internal
} // namespace ahoy
