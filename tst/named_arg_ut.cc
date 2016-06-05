#include "ahoy/named_arg.h"

#include <gtest/gtest.h>

namespace {

const char kName[] = "name";
const char kDescription[] = "description";

} // namespace

namespace ahoy {

class NamedArgTest : public ::testing::Test {
  public:
    NamedArgTest() : named_arg_(NamedArg(kName, kDescription)) {}

  protected:
    const NamedArg named_arg_;
};

TEST_F(NamedArgTest, Name) {
    EXPECT_EQ(kName, named_arg_.name());
}

TEST_F(NamedArgTest, Description) {
    EXPECT_EQ(kDescription, named_arg_.description());
}

} // namespace ahoy
