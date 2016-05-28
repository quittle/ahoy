#include "ahoy/parser.h"

#include "ahoy/arg.h"
#include "ahoy/param.h"
#include "ahoy/parse_result.h"

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
    const ParseResult<int> result_empty = Parser<int>({}).Parse(1, args);
    EXPECT_TRUE(result_empty);
    EXPECT_EQ(0, result_empty.params().size());
    EXPECT_EQ(0, result_empty.errors().size());

    args[0] = kExecutable;
    const ParseResult<int> result_executable = Parser<int>({}).Parse(1, args);
    EXPECT_TRUE(result_executable);
    EXPECT_EQ(0, result_executable.params().size());
    EXPECT_EQ(0, result_executable.errors().size());
}

TEST(Parser, Simple_ValidShort_Equals) {
    char const * const args[2] = { kExecutable, "-f=bar" };
    const ParseResult<int> result =
            Parser<int>({{ 0, {{"f"}, {}, false, kDescription, kDefaultValue }}}).Parse(2, args);
    EXPECT_TRUE(result);
    const std::map<int, Param> expected_params = {{ 0, "bar" }};
    EXPECT_EQ(expected_params, result.params());
    EXPECT_TRUE(result.errors().empty());
}

TEST(Parser, Simple_ValidShort_Space) {
    char const * const args[3] = { kExecutable, "-f", "bar" };
    const ParseResult<int> result =
            Parser<int>({{ 0, {{"f"}, {}, false, kDescription, kDefaultValue }}}).Parse(3, args);
    EXPECT_TRUE(result);
    const std::map<int, Param> expected_params = {{ 0, "bar" }};
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Simple_ValidLong_Equals) {
    char const * const args[2] = { kExecutable, "--foo=bar" };
    const ParseResult<int> result =
            Parser<int>({{ 0, {{}, {"foo"}, false, kDescription, kDefaultValue }}}).Parse(2, args);
    EXPECT_TRUE(result);
    const std::map<int, Param> expected_params = {{ 0, "bar" }};
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Simple_ValidLong_Space) {
    char const * const args[3] = { kExecutable, "--foo", "bar" };
    const ParseResult<int> result =
            Parser<int>({{ 0, {{}, {"foo"}, false, kDescription, kDefaultValue }}}).Parse(3, args);
    EXPECT_TRUE(result);
    const std::map<int, Param> expected_params = {{ 0, "bar" }};
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Simple_Invalid) {
    char const * const args[2] = { kExecutable, "-f" };
    const std::map<int, Param> expected_defaulted_params = {{ 0, kDefaultValue }};
    const ParseResult<int> both_empty_result =
            Parser<int>({{ 0, {{}, {}, false, kDescription, kDefaultValue }}}).Parse(2, args);
    EXPECT_FALSE(both_empty_result);
    EXPECT_EQ(expected_defaulted_params, both_empty_result.params());
    EXPECT_EQ(1, both_empty_result.errors().size());

    const ParseResult<int> short_result =
            Parser<int>({{ 0, {{"f"}, {}, false, kDescription, kDefaultValue }}}).Parse(2, args);
    EXPECT_FALSE(short_result);
    EXPECT_EQ(expected_defaulted_params, short_result.params());
    EXPECT_EQ(1, short_result.errors().size());

    const ParseResult<int> long_result =
            Parser<int>({{ 0, {{}, {"f"}, false, kDescription, kDefaultValue }}}).Parse(2, args);
    EXPECT_FALSE(long_result);
    EXPECT_EQ(expected_defaulted_params, long_result.params());
    EXPECT_EQ(1, long_result.errors().size());
}

TEST(Parser, Required) {
    char const * const args[3] = { kExecutable, "-rf", "value" };
    const ParseResult<int> result =
            Parser<int>({{ 0, {{"rf"}, {"requiredField"}, true, kDescription, kDefaultValue }}})
                    .Parse(3, args);
    EXPECT_TRUE(result);
    const std::map<int, Param> expected_params = {{ 0, "value" }};
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Required_Missing) {
    char const * const args[1] = { kExecutable };
    const ParseResult<int> result =
            Parser<int>({{ 0, {{"requiredField"}, {}, true, kDescription, kDefaultValue }}})
                    .Parse(1, args);
    EXPECT_FALSE(result);
    EXPECT_TRUE(result.params().empty());
    EXPECT_EQ(1, result.errors().size());
}

TEST(Parser, DefaultValue) {
    char const * const args[1] = { kExecutable };
    const ParseResult<int> result =
            Parser<int>({{ 0, {{"mf"}, {"missingFiled"}, false, kDescription, kDefaultValue }}})
                    .Parse(1, args);
    const std::map<int, Param> expected_params = {{ 0, kDefaultValue }};
    EXPECT_TRUE(result);
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Dupes) {}

TEST(Parser, Multiple) {
    char const * const args[6] =
            { kExecutable, "-f=foo", "--bar", "baz boop", "-hey", "hello=world" };
    const ParseResult<int> result = Parser<int>({
        { 0, {{"f"}, {"long-f"}, false, kDescription, kDefaultValue }},
        { 1, {{"b"}, {"bar"}, true, kDescription, kDefaultValue }},
        { 2, {{"m"}, {"missing"}, false, kDescription, kDefaultValue }},
        { 3, {{"h"}, {"hey"}, true, kDescription, kDefaultValue }}
    }).Parse(1, args);
    const std::map<int, Param> expected_params = {
        { 0, "foo" },
        { 1, "baz boop" },
        { 2, kDefaultValue },
        { 3, "hello=world" }
    };
    EXPECT_TRUE(result);
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
    for (auto error : result.errors()) {
        std::cout << error << std::endl;
    }

}

TEST(Parser, Help) {}

} // namespace ahoy
