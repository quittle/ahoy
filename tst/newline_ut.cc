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
