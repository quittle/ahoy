// Copyright (c) 2016 Dustin Toff
// Licensed under Apache License v2.0

#include "ahoy/parser.h"

#include <gtest/gtest.h>

// These tests require manual intervention to run.

namespace {

const char kName[] = "name";

} // namespace

namespace ahoy {

// To test, uncomment each line individually to verify static_asserts will cause build errors
TEST(Parser_ManualTest, FlagRequired_StaticAssertFailure) {
    bool param = false;

    // Required to avoid compiler warnings
    EXPECT_FALSE(param);

    // Parser().AddNamedParam(&param, Flag(), Required());
    // Parser().AddNamedParam(&param, Flag(), Name(kName), Required());
    // Parser().AddNamedParam(&param, Required(), Flag());
    // Parser().AddNamedParam(&param, Required(), Name(kName), Flag());
}

} // namespace ahoy
