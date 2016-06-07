#include "ahoy/formal_parameter.h"

#include <set>

#include <gtest/gtest.h>

namespace {

const std::set<std::string>& kShortForms = { "s", "h", "ort", "forms" };
const std::set<std::string>& kLongForms = { "long", "forms" };
const char kDescription[] = "description";
const char kDefaultValue[] = "default value";

} // namespace

namespace ahoy {

class FormalParameterTest : public ::testing::Test {
  public:
    FormalParameterTest() :
        flag_param_(FormalParameter(kShortForms, kLongForms, kDescription, true)),
        required_switch_param_(FormalParameter(kShortForms, kLongForms, kDescription, false)),
        non_required_switch_param_(FormalParameter(kShortForms, kLongForms, kDescription,
                                                   kDefaultValue)) {}

  protected:
    const FormalParameter flag_param_;
    const FormalParameter required_switch_param_;
    const FormalParameter non_required_switch_param_;
};

TEST_F(FormalParameterTest, HasShortForm) {
    EXPECT_TRUE(flag_param_.HasShortForm("s"));
    EXPECT_TRUE(required_switch_param_.HasShortForm("s"));
    EXPECT_TRUE(non_required_switch_param_.HasShortForm("s"));
    EXPECT_FALSE(flag_param_.HasShortForm("long"));
    EXPECT_FALSE(required_switch_param_.HasShortForm("long"));
    EXPECT_FALSE(non_required_switch_param_.HasShortForm("long"));
}

TEST_F(FormalParameterTest, HasLongForms) {
    EXPECT_FALSE(flag_param_.HasLongForm("s"));
    EXPECT_FALSE(required_switch_param_.HasLongForm("s"));
    EXPECT_FALSE(non_required_switch_param_.HasLongForm("s"));
    EXPECT_TRUE(flag_param_.HasLongForm("long"));
    EXPECT_TRUE(required_switch_param_.HasLongForm("long"));
    EXPECT_TRUE(non_required_switch_param_.HasLongForm("long"));
}

TEST_F(FormalParameterTest, IsFlag) {
    EXPECT_TRUE(flag_param_.IsFlag());
    EXPECT_FALSE(required_switch_param_.IsFlag());
    EXPECT_FALSE(non_required_switch_param_.IsFlag());
}

TEST_F(FormalParameterTest, ShortForms) {
    EXPECT_EQ(kShortForms, flag_param_.short_forms());
    EXPECT_EQ(kShortForms, required_switch_param_.short_forms());
    EXPECT_EQ(kShortForms, non_required_switch_param_.short_forms());
}

TEST_F(FormalParameterTest, LongForms) {
    EXPECT_EQ(kLongForms, flag_param_.long_forms());
    EXPECT_EQ(kLongForms, required_switch_param_.long_forms());
    EXPECT_EQ(kLongForms, non_required_switch_param_.long_forms());
}

TEST_F(FormalParameterTest, Required) {
    EXPECT_FALSE(flag_param_.IsRequired());
    EXPECT_TRUE(required_switch_param_.IsRequired());
    EXPECT_FALSE(non_required_switch_param_.IsRequired());
}

TEST_F(FormalParameterTest, Description) {
    EXPECT_EQ(kDescription, flag_param_.description());
    EXPECT_EQ(kDescription, required_switch_param_.description());
    EXPECT_EQ(kDescription, non_required_switch_param_.description());
}

TEST_F(FormalParameterTest, DefaultValue) {
    EXPECT_EQ(nullptr, flag_param_.default_value());
    EXPECT_EQ(nullptr, required_switch_param_.default_value());
    EXPECT_EQ(kDefaultValue, *non_required_switch_param_.default_value());
}

} // namespace ahoy
