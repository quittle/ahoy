// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/newline.h"

#include <sstream>

#include <gtest/gtest.h>

namespace ahoy {
namespace internal {

TEST(Newline, NewlineToStream_Posix) {
    EXPECT_EQ("\n", NewlineToString(Newline::POSIX));
}

TEST(Newline, Stream_Posix) {
    std::stringstream ss;
    ss << Newline::POSIX;
    EXPECT_EQ("\n", ss.str());
}

TEST(Newline, NewlineToStream_Windows) {
    EXPECT_EQ("\r\n", NewlineToString(Newline::WINDOWS));
}

TEST(Newline, Stream_Windows) {
    std::stringstream ss;
    ss << Newline::WINDOWS;
    EXPECT_EQ("\r\n", ss.str());
}

} // namespace internal
} // namespace ahoy
