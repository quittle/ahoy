#include "ahoy/parser.h"

#include <gtest/gtest.h>

namespace {

const char kExecutable[] = "./executable";
const char kDescription[] = "description";
const char kDefaultValue[] = "default value";

} // namespace

namespace ahoy {

TEST(Parser, Template) {
    const Arg arg = {{}, {}, false, kDescription, kDefaultValue};
    Parser<char>({{ 'c', arg }});
    Parser<int>({{ 0, arg }});
    Parser<long>({{ 0l, arg }});
    Parser<std::string>({{ "string", arg }});
}

TEST(Parser, Empty) {
    char const * args[1] = { "" };
    Parser<int>::Parameters params;
    EXPECT_TRUE(Parser<int>({}).Parse(1, args, params));
    EXPECT_TRUE(params.empty());
    args[0] = kExecutable;
    EXPECT_TRUE(Parser<int>({}).Parse(1, args, params));
    EXPECT_TRUE(params.empty());
}

TEST(Parser, Simple_ValidShort_Equals) {
    char const * const args[2] = { kExecutable, "-f=bar" };
    Parser<int>::Parameters params;
    EXPECT_TRUE(Parser<int>({{ 0, {{"f"}, {}, false, kDescription, kDefaultValue }}}).Parse(2, args, params));
    Parser<int>::Parameters expected_params = {{ 0, "bar" }};
    EXPECT_EQ(expected_params, params);
}

TEST(Parser, Simple_ValidShort_Space) {
    char const * const args[3] = { kExecutable, "-f", "bar" };
    Parser<int>::Parameters params;
    EXPECT_TRUE(Parser<int>({{ 0, {{"f"}, {}, false, kDescription, kDefaultValue }}}).Parse(3, args, params));
    Parser<int>::Parameters expected_params = {{ 0, "bar" }};
    EXPECT_EQ(expected_params, params);
}

TEST(Parser, Simple_ValidLong_Equals) {
    char const * const args[2] = { kExecutable, "--foo=bar" };
    Parser<int>::Parameters params;
    EXPECT_TRUE(Parser<int>({{ 0, {{}, {"foo"}, false, kDescription, kDefaultValue }}}).Parse(2, args, params));
    Parser<int>::Parameters expected_params = {{ 0, "bar" }};
    EXPECT_EQ(expected_params, params);
}

TEST(Parser, Simple_ValidLong_Space) {
    char const * const args[3] = { kExecutable, "--foo", "bar" };
    Parser<int>::Parameters params;
    EXPECT_TRUE(Parser<int>({{ 0, {{}, {"foo"}, false, kDescription, kDefaultValue }}}).Parse(3, args, params));
    Parser<int>::Parameters expected_params = {{ 0, "bar" }};
    EXPECT_EQ(expected_params, params);
}

TEST(Parser, Simple_Invalid) {
    char const * const args[2] = { kExecutable, "-f" };
    Parser<int>::Parameters params;
    EXPECT_FALSE(Parser<int>({{ 0, {{}, {}, false, kDescription, kDefaultValue }}}).Parse(2, args, params));
    EXPECT_TRUE(params.empty());
    EXPECT_FALSE(Parser<int>({{ 0, {{"f"}, {}, false, kDescription, kDefaultValue }}}).Parse(2, args, params));
    EXPECT_TRUE(params.empty());
    EXPECT_FALSE(Parser<int>({{ 0, {{}, {"f"}, false, kDescription, kDefaultValue }}}).Parse(2, args, params));
    EXPECT_TRUE(params.empty());
}

TEST(Parser, Required) {
    char const * const args[3] = { kExecutable, "-rf", "value" };
    Parser<int>::Parameters params;
    EXPECT_TRUE(Parser<int>({{ 0, {{"rf"}, {"requiredField"}, true, kDescription, kDefaultValue }}}).Parse(3, args, params));
    Parser<int>::Parameters expected_params = {{ 0, Param("value") }};
    EXPECT_EQ(expected_params, params);
}

TEST(Parser, Required_Missing) {
    char const * const args[1] = { kExecutable };
    Parser<int>::Parameters params;
    EXPECT_FALSE(Parser<int>({{ 0, {{"requiredField"}, {}, true, kDescription, kDefaultValue }}}).Parse(1, args, params));
    EXPECT_TRUE(params.empty());
}

} // namespace ahoy
