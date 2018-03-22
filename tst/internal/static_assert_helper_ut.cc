// Copyright (c) 2016, 2018 Dustin Toff
// Licensed under Apache License v2.0

#include "ahoy/internal/static_assert_helper.h"

#include <gtest/gtest.h>

namespace {

// Necessary abstraction as templates cause issues with macro parsing
void ExpectTrue(const bool value) {
    EXPECT_TRUE(value);
}

// Necessary abstraction as templates cause issues with macro parsing
void ExpectFalse(const bool value) {
    EXPECT_FALSE(value);
}

} // namespace

namespace ahoy {
namespace internal {

TEST(StaticAssertHelper, DoesContainType1) {
    ExpectTrue(does_contain_type_1<int, int>::value);
    ExpectTrue(does_contain_type_1<int, bool, int>::value);
    ExpectTrue(does_contain_type_1<int, int, bool>::value);
    ExpectTrue(does_contain_type_1<int, bool, int, bool>::value);
    ExpectTrue(does_contain_type_1<int, int, int>::value);

    ExpectFalse(does_contain_type_1<int>::value);
    ExpectFalse(does_contain_type_1<int, bool>::value);
    ExpectFalse(does_contain_type_1<int, short, bool>::value);
}

TEST(StaticAssertHelper, DoesContainType2) {
    ExpectTrue(does_contain_type_2<int, bool, int, bool>::value);
    ExpectTrue(does_contain_type_2<int, bool, bool, int>::value);
    ExpectTrue(does_contain_type_2<int, bool, bool, short, int>::value);
    ExpectTrue(does_contain_type_2<int, bool, short, bool, short, int>::value);
    ExpectTrue(does_contain_type_2<int, bool, int, bool, short, long>::value);

    ExpectFalse(does_contain_type_2<int, int>::value);
    ExpectFalse(does_contain_type_2<int, bool>::value);
    ExpectFalse(does_contain_type_2<int, bool, short>::value);
    ExpectFalse(does_contain_type_2<int, bool, int>::value);
    ExpectFalse(does_contain_type_2<int, bool, bool>::value);
    ExpectFalse(does_contain_type_2<int, bool, int, short>::value);
    ExpectFalse(does_contain_type_2<int, bool, short, bool>::value);
    ExpectFalse(does_contain_type_2<int, bool, short, long>::value);
}

} // namespace internal
} // namespace ahoy
