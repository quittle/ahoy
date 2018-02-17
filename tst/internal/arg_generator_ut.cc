// Copyright (c) 2016, 2018 Dustin Doloff
// Licensed under Apache License v2.0

#include <cstdarg>

#include "ahoy/internal/arg_generator.h"

#include <gtest/gtest.h>

namespace {

const char kProgram[] = "./prog";
char const * const kNoArgs[1] = { kProgram };

} // namespace

namespace ahoy {
namespace internal {

TEST(ArgGenerator, Peek_Empty) {
    ArgGenerator::Result result = ArgGenerator(0, {}).Peek();
    EXPECT_EQ(FlagArg::INVALID, result.flag_arg);
    EXPECT_EQ(NamedArg::INVALID, result.named_arg);
    EXPECT_EQ(PositionalArg::INVALID, result.positional_arg);

    result = ArgGenerator(1, kNoArgs).Peek();
    EXPECT_EQ(FlagArg::INVALID, result.flag_arg);
    EXPECT_EQ(NamedArg::INVALID, result.named_arg);
    EXPECT_EQ(PositionalArg::INVALID, result.positional_arg);
}

TEST(ArgGenerator, Peek_SingleFlag) {
    char const * const args[2] = { kProgram, "--flag" };
    const ArgGenerator::Result result = ArgGenerator(2, args).Peek();
    EXPECT_EQ(FlagArg(ArgSize::LONG, "flag"), result.flag_arg);
    EXPECT_EQ(NamedArg::INVALID, result.named_arg);
    EXPECT_EQ(PositionalArg("--flag"), result.positional_arg);
}

TEST(ArgGenerator, Peek_Switch_SinglePart) {
    char const * const args[2] = { kProgram, "--switch=value" };
    const ArgGenerator::Result result = ArgGenerator(2, args).Peek();
    EXPECT_EQ(FlagArg(ArgSize::LONG, "switch=value"), result.flag_arg);
    EXPECT_EQ(NamedArg(ArgSize::LONG, "switch", "value", true), result.named_arg);
    EXPECT_EQ(PositionalArg("--switch=value"), result.positional_arg);
}

TEST(ArgGenerator, Peek_Switch_TwoParts) {
    char const * const args[3] = { kProgram, "--switch", "value" };
    const ArgGenerator::Result result = ArgGenerator(3, args).Peek();
    EXPECT_EQ(FlagArg(ArgSize::LONG, "switch"), result.flag_arg);
    EXPECT_EQ(NamedArg(ArgSize::LONG, "switch", "value", false), result.named_arg);
    EXPECT_EQ(PositionalArg("--switch"), result.positional_arg);
}

TEST(ArgGenerator, Peek_Positional) {
    char const * const args[3] = { kProgram, "positional" };
    const ArgGenerator::Result result = ArgGenerator(3, args).Peek();
    EXPECT_EQ(FlagArg::INVALID, result.flag_arg);
    EXPECT_EQ(NamedArg::INVALID, result.named_arg);
    EXPECT_EQ(PositionalArg("positional"), result.positional_arg);
}

TEST(ArgGenerator, AdvanceArgs_NoCrash) {
    ArgGenerator generator(1, kNoArgs);

    ASSERT_FALSE(generator.Available());

    for (int i = 0; i < 10; i++) {
        generator.AdvanceFlagArg();
        ASSERT_FALSE(generator.Available());
        generator.AdvanceNamedArg();
        ASSERT_FALSE(generator.Available());
        generator.AdvancePositionalArg();
        ASSERT_FALSE(generator.Available());
    }
}

TEST(ArgGenerator, AdvanceNamedArg_NotPossible) {
    char const * const args[2] = { kProgram, "--flag" };
    ArgGenerator generator(2, args);

    ASSERT_TRUE(generator.Available());
    ASSERT_NE(FlagArg::INVALID, generator.Peek().flag_arg);

    generator.AdvanceNamedArg();

    ASSERT_NE(FlagArg::INVALID, generator.Peek().flag_arg);
    ASSERT_TRUE(generator.Available());

    generator.AdvanceFlagArg();

    ASSERT_EQ(FlagArg::INVALID, generator.Peek().flag_arg);
    ASSERT_FALSE(generator.Available());
}

TEST(ArgGenerator, Available_False) {
    EXPECT_FALSE(ArgGenerator(0, {}).Available());
    EXPECT_FALSE(ArgGenerator(1, kNoArgs).Available());
}

TEST(ArgGenerator, Available) {
    char const * const args[5] = { kProgram, "--flag", "--switch", "true", "positional" };
    ArgGenerator generator(5, args);
    EXPECT_TRUE(generator.Available());

    generator.AdvanceFlagArg();

    EXPECT_TRUE(generator.Available());

    generator.AdvanceNamedArg();

    EXPECT_TRUE(generator.Available());

    generator.AdvancePositionalArg();

    EXPECT_FALSE(generator.Available());
}

} // namespace internal
} // namespace ahoy
