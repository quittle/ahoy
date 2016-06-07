#include "ahoy/arg.h"

#include <set>

#include <gtest/gtest.h>

namespace {

const std::set<std::string>& kShortForms = { "s", "h", "ort", "forms" };
const std::set<std::string>& kLongForms = { "long", "forms" };
const char kDescription[] = "description";
const char kDefaultValue[] = "default value";

} // namespace

namespace ahoy {

class ArgTest : public ::testing::Test {
  public:
    ArgTest() :
        flag_arg_(Arg(kShortForms, kLongForms, kDescription, true)),
        required_switch_arg_(Arg(kShortForms, kLongForms, kDescription, false)),
        non_required_switch_arg_(Arg(kShortForms, kLongForms, kDescription, kDefaultValue)) {}

  protected:
    const Arg flag_arg_;
    const Arg required_switch_arg_;
    const Arg non_required_switch_arg_;
};

TEST_F(ArgTest, HasShortForm) {
    EXPECT_TRUE(flag_arg_.HasShortForm("s"));
    EXPECT_TRUE(required_switch_arg_.HasShortForm("s"));
    EXPECT_TRUE(non_required_switch_arg_.HasShortForm("s"));
    EXPECT_FALSE(flag_arg_.HasShortForm("long"));
    EXPECT_FALSE(required_switch_arg_.HasShortForm("long"));
    EXPECT_FALSE(non_required_switch_arg_.HasShortForm("long"));
}

TEST_F(ArgTest, HasLongForms) {
    EXPECT_FALSE(flag_arg_.HasLongForm("s"));
    EXPECT_FALSE(required_switch_arg_.HasLongForm("s"));
    EXPECT_FALSE(non_required_switch_arg_.HasLongForm("s"));
    EXPECT_TRUE(flag_arg_.HasLongForm("long"));
    EXPECT_TRUE(required_switch_arg_.HasLongForm("long"));
    EXPECT_TRUE(non_required_switch_arg_.HasLongForm("long"));
}

TEST_F(ArgTest, IsFlag) {
    EXPECT_TRUE(flag_arg_.IsFlag());
    EXPECT_FALSE(required_switch_arg_.IsFlag());
    EXPECT_FALSE(non_required_switch_arg_.IsFlag());
}

TEST_F(ArgTest, ShortForms) {
    EXPECT_EQ(kShortForms, flag_arg_.short_forms());
    EXPECT_EQ(kShortForms, required_switch_arg_.short_forms());
    EXPECT_EQ(kShortForms, non_required_switch_arg_.short_forms());
}

TEST_F(ArgTest, LongForms) {
    EXPECT_EQ(kLongForms, flag_arg_.long_forms());
    EXPECT_EQ(kLongForms, required_switch_arg_.long_forms());
    EXPECT_EQ(kLongForms, non_required_switch_arg_.long_forms());
}

TEST_F(ArgTest, Required) {
    EXPECT_FALSE(flag_arg_.IsRequired());
    EXPECT_TRUE(required_switch_arg_.IsRequired());
    EXPECT_FALSE(non_required_switch_arg_.IsRequired());
}

TEST_F(ArgTest, Description) {
    EXPECT_EQ(kDescription, flag_arg_.description());
    EXPECT_EQ(kDescription, required_switch_arg_.description());
    EXPECT_EQ(kDescription, non_required_switch_arg_.description());
}

TEST_F(ArgTest, DefaultValue) {
    EXPECT_EQ(nullptr, flag_arg_.default_value());
    EXPECT_EQ(nullptr, required_switch_arg_.default_value());
    EXPECT_EQ(kDefaultValue, *non_required_switch_arg_.default_value());
}

} // namespace ahoy
