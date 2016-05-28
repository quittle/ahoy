#include "ahoy/arg.h"

#include <gtest/gtest.h>

namespace {

const std::set<std::string>& kShortForms = { "s", "h", "ort", "forms" };
const std::set<std::string>& kLongForms = { "long", "forms" };
const std::string kDescription = "description";
const std::string kDefaultValue = "default value";

} // namespace

namespace ahoy {

class ArgTest : public ::testing::Test {
  public:
    ArgTest() : arg_(Arg(kShortForms, kLongForms, true, kDescription, kDefaultValue)) {}

  protected:
    const Arg arg_;
};

TEST_F(ArgTest, ShortForms) {
    EXPECT_EQ(kShortForms, arg_.short_forms());
}

TEST_F(ArgTest, LongForms) {
    EXPECT_EQ(kLongForms, arg_.long_forms());
}

TEST_F(ArgTest, Required) {
    EXPECT_EQ(true, Arg(kShortForms, kLongForms, true, kDescription, kDefaultValue).required());
    EXPECT_EQ(false, Arg(kShortForms, kLongForms, false, kDescription, kDefaultValue).required());
}

TEST_F(ArgTest, Description) {
    EXPECT_EQ(kDescription, arg_.description());
}

TEST_F(ArgTest, DefaultValue) {
    EXPECT_EQ(kDefaultValue, arg_.default_value());
}

} // namespace ahoy