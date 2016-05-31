#include "ahoy/arg.h"

#include <set>

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
    ArgTest() :
        flag_arg_(Arg(kShortForms, kLongForms, kDescription)),
        string_arg_(Arg(kShortForms, kLongForms, kDescription, true, kDefaultValue)) {}

  protected:
    const Arg flag_arg_;
    const Arg string_arg_;
};

TEST_F(ArgTest, HasShortForm) {
    EXPECT_TRUE(flag_arg_.HasShortForm("s"));
    EXPECT_TRUE(string_arg_.HasShortForm("s"));
    EXPECT_FALSE(flag_arg_.HasShortForm("long"));
    EXPECT_FALSE(string_arg_.HasShortForm("long"));
}

TEST_F(ArgTest, HasLongForms) {
    EXPECT_FALSE(flag_arg_.HasLongForm("s"));
    EXPECT_FALSE(string_arg_.HasLongForm("s"));
    EXPECT_TRUE(flag_arg_.HasLongForm("long"));
    EXPECT_TRUE(string_arg_.HasLongForm("long"));
}

TEST_F(ArgTest, IsFlag) {
    EXPECT_TRUE(flag_arg_.IsFlag());
    EXPECT_FALSE(string_arg_.IsFlag());
}

TEST_F(ArgTest, ShortForms) {
    EXPECT_EQ(kShortForms, flag_arg_.short_forms());
    EXPECT_EQ(kShortForms, string_arg_.short_forms());
}

TEST_F(ArgTest, LongForms) {
    EXPECT_EQ(kLongForms, flag_arg_.long_forms());
    EXPECT_EQ(kLongForms, string_arg_.long_forms());
}

TEST_F(ArgTest, Required) {
    EXPECT_TRUE(Arg(kShortForms, kLongForms, kDescription, true, kDefaultValue).required());
    EXPECT_FALSE(Arg(kShortForms, kLongForms, kDescription, false, kDefaultValue).required());
}

TEST_F(ArgTest, Description) {
    EXPECT_EQ(kDescription, flag_arg_.description());
    EXPECT_EQ(kDescription, string_arg_.description());
}

TEST_F(ArgTest, DefaultValue) {
    EXPECT_EQ(nullptr, flag_arg_.default_value());
    EXPECT_EQ(kDefaultValue, *string_arg_.default_value());
}

} // namespace ahoy
