// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/newline.h"

#include <gtest/gtest.h>

namespace ahoy {

TEST(NEWLINE, POSIX) {
    EXPECT_EQ("\n", NewlineToString(Newline::POSIX));
}

TEST(NEWLINE, WINDOWS) {
    EXPECT_EQ("\r\n", NewlineToString(Newline::WINDOWS));
}

} // namespace ahoy
