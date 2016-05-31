#include "ahoy/parser.h"

#include "ahoy/arg.h"
#include "ahoy/newline.h"
#include "ahoy/param.h"
#include "ahoy/parse_result.h"

#include <gtest/gtest.h>

namespace {

const char kExecutable[] = "./executable";
const char kDescription[] = "description";
const char kDefaultValue[] = "default value";

} // namespace

namespace ahoy {

TEST(Parser, Create_Valid) {
    Parser<int> p = Parser<int>::Create({
        { 0, {{}, {}, kDescription, false, kDefaultValue} },
        { 1, {{"f"}, {"flag"}, kDescription, true, kDefaultValue} },
        { 2, {{"flag"}, {"flagflag"}, kDescription, true, kDefaultValue} }
    });
    EXPECT_TRUE(p);
}

TEST(Parser, Create_Invalid) {
    Parser<int> p = Parser<int>::Create({
        { 0, {{}, {}, kDescription, false, kDefaultValue} },
        { 1, {{"f"}, {"flag"}, kDescription, true, kDefaultValue} },
        { 2, {{"f"}, {"flag"}, kDescription, true, kDefaultValue} }
    });
    EXPECT_FALSE(p);
}

TEST(Parser, Template) {
    const Arg arg = {{}, {}, kDescription, false, kDefaultValue};
    Parser<char>::Create({{ 'c', arg }});
    Parser<int>::Create({{ 0, arg }});
    Parser<long>::Create({{ 0l, arg }});
    Parser<std::string>::Create({{ "string", arg }});
}

TEST(Parser, Help_Windows) {
    EXPECT_EQ(
        "-f -foo --foo (Defaults to bar) - Help message 1\r\n"
        "-w (Required) - Help message 2\r\n"
        "--long-arg --long-arg-alt (Defaults to true) - Help message 3\r\n"
        "--flag (Flag) - Enables the flag\r\n",
        Parser<int>::Create({
            { 0, { {"f", "foo"}, {"foo"}, "Help message 1", false, "bar" } },
            { 1, { {"w"}, {}, "Help message 2", true, "Should not be used" } },
            { 2, { {}, {"long-arg", "long-arg-alt"}, "Help message 3", false, "true" } },
            { 3, { {}, {"flag"}, "Enables the flag" }}
        }).HelpMessage(Newline::WINDOWS)
    );
}

TEST(Parser, Help_Posix) {
    EXPECT_EQ(
        "-f -foo --foo (Defaults to bar) - Help message 1\n"
        "-w (Required) - Help message 2\n"
        "--long-arg --long-arg-alt (Defaults to true) - Help message 3\n"
        "--flag (Flag) - Enables the flag\n",
        Parser<int>::Create({
            { 0, { {"f", "foo"}, {"foo"}, "Help message 1", false, "bar" } },
            { 1, { {"w"}, {}, "Help message 2", true, "Should not be used" } },
            { 2, { {}, {"long-arg", "long-arg-alt"}, "Help message 3", false, "true" } },
            { 3, { {}, {"flag"}, "Enables the flag" }}
        }).HelpMessage(Newline::POSIX)
    );
}

TEST(Parser, InvalidParser) {
    char const * const args[2] = { kExecutable, "-f=bar" };
    const Parser<int> parser = Parser<int>::Create({
                { 0, {{"f"}, {}, kDescription, false, kDefaultValue }},
                { 1, {{"f"}, {}, kDescription, false, kDefaultValue }},  // Duplicate
            });
    EXPECT_FALSE(parser);
    const ParseResult<int> result = parser.Parse(2, args);
    EXPECT_FALSE(result);
    const std::map<int, Param> expected_params = {{ 0, "bar" }, { 1, kDefaultValue }};
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(1, result.errors().size());
}

TEST(Parser, MissingExecutable) {
    char const * const * const args = nullptr;
    const ParseResult<int> result = Parser<int>::Create({}).Parse(0, args);
    EXPECT_FALSE(result);
    EXPECT_EQ(0, result.params().size());
    EXPECT_EQ(1, result.errors().size());
}

TEST(Parser, Parse_Empty) {
    char const * args[1] = { "" };
    const ParseResult<int> result_empty = Parser<int>::Create({}).Parse(1, args);
    EXPECT_TRUE(result_empty);
    EXPECT_EQ(0, result_empty.params().size());
    EXPECT_EQ(0, result_empty.errors().size());

    args[0] = kExecutable;
    const ParseResult<int> result_executable = Parser<int>::Create({}).Parse(1, args);
    EXPECT_TRUE(result_executable);
    EXPECT_EQ(0, result_executable.params().size());
    EXPECT_EQ(0, result_executable.errors().size());
}

TEST(Parser, Parse_Simple_ValidShort_Equals) {
    char const * const args[2] = { kExecutable, "-f=bar" };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, { {"f"}, {}, kDescription, false, kDefaultValue } }
            }).Parse(2, args);
    EXPECT_TRUE(result);
    const std::map<int, Param> expected_params = {{ 0, "bar" }};
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Parse_Simple_ValidShort_Space) {
    char const * const args[3] = { kExecutable, "-f", "bar" };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, { {"f"}, {}, kDescription, false, kDefaultValue } }
            }).Parse(3, args);
    EXPECT_TRUE(result);
    const std::map<int, Param> expected_params = {{ 0, "bar" }};
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Simple_ValidLong_Equals) {
    char const * const args[2] = { kExecutable, "--foo=bar" };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, { {}, {"foo"}, kDescription, false, kDefaultValue } }
            }).Parse(2, args);
    EXPECT_TRUE(result);
    const std::map<int, Param> expected_params = {{ 0, "bar" }};
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Simple_ValidLong_Space) {
    char const * const args[3] = { kExecutable, "--foo", "bar" };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, { {}, {"foo"}, kDescription, false, kDefaultValue } }
            }).Parse(3, args);
    EXPECT_TRUE(result);
    const std::map<int, Param> expected_params = {{ 0, "bar" }};
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Parse_Simple_Invalid) {
    char const * const args[2] = { kExecutable, "-f" };
    const std::map<int, Param> expected_defaulted_params = {{ 0, kDefaultValue }};
    const ParseResult<int> both_empty_result = Parser<int>::Create({
                { 0, {{}, {}, kDescription, false, kDefaultValue } }
            }).Parse(2, args);
    EXPECT_FALSE(both_empty_result);
    EXPECT_EQ(expected_defaulted_params, both_empty_result.params());
    EXPECT_EQ(1, both_empty_result.errors().size());

    const ParseResult<int> short_result = Parser<int>::Create({
                { 0, { {"f"}, {}, kDescription, false, kDefaultValue } }
            }).Parse(2, args);
    EXPECT_FALSE(short_result);
    EXPECT_EQ(expected_defaulted_params, short_result.params());
    EXPECT_EQ(1, short_result.errors().size());

    const ParseResult<int> long_result = Parser<int>::Create({
                { 0, { {}, {"f"}, kDescription, false, kDefaultValue } }
            }).Parse(2, args);
    EXPECT_FALSE(long_result);
    EXPECT_EQ(expected_defaulted_params, long_result.params());
    EXPECT_EQ(1, long_result.errors().size());
}

TEST(Parser, Parse_Required) {
    char const * const args[3] = { kExecutable, "-rf", "value" };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, { {"rf"}, {"requiredField"}, kDescription, true, kDefaultValue } }
            }).Parse(3, args);
    EXPECT_TRUE(result);
    const std::map<int, Param> expected_params = {{ 0, "value" }};
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Required_Missing) {
    char const * const args[1] = { kExecutable };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, { {"requiredField"}, {}, kDescription, true, kDefaultValue } }
            }).Parse(1, args);
    EXPECT_FALSE(result);
    EXPECT_EQ(0, result.params().size());
    EXPECT_EQ(1, result.errors().size());
}

TEST(Parser, Parse_DefaultValue) {
    char const * const args[1] = { kExecutable };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, { {"mf"}, {"missingFiled"}, kDescription, false, kDefaultValue } }
            }).Parse(1, args);
    const std::map<int, Param> expected_params = {{ 0, kDefaultValue }};
    EXPECT_TRUE(result);
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Parse_Flag_Missing) {
    char const * const args[1] = { kExecutable };
    const ParseResult<int> result =
            Parser<int>::Create({{ 0, {{"f"}, {"flag"}, kDescription }}})
                    .Parse(1, args);
    const std::map<int, Param> expected_params = {{ 0, false }};
    EXPECT_TRUE(result);
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Parse_Flag) {
    char const * const args[2] = { kExecutable, "--flag" };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, { {"f"}, {"flag"}, kDescription } }
            }).Parse(2, args);
    const std::map<int, Param> expected_params = {{ 0, true }};
    EXPECT_TRUE(result);
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());

}

TEST(Parser, Parse_Duplicates) {
    char const * const args[6] =
        { kExecutable, "-f", "value", "--field=value_2", "-flag", "--flag" };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, {{"f"}, {"field"}, kDescription, true, kDefaultValue }},
                { 1, {{"flag"}, {"flag"}, kDescription}}
            }).Parse(6, args);
    EXPECT_FALSE(result);
    const std::map<int, Param> expected_params = {{ 0, "value" }, { 1, true }};
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(2, result.errors().size());
}

TEST(Parser, Parse_Multiple) {
    char const * const args[6] =
            { kExecutable, "-f=foo", "--bar", "baz boop", "-hey", "hello=world" };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, {{"f"}, {"long-f"}, kDescription, false, kDefaultValue }},
                { 1, {{"b"}, {"bar"}, kDescription, true, kDefaultValue }},
                { 2, {{"m"}, {"missing"}, kDescription, false, kDefaultValue }},
                { 3, {{"hey"}, {"hey"}, kDescription, true, kDefaultValue }}
            }).Parse(6, args);
    const std::map<int, Param> expected_params = {
        { 0, "foo" },
        { 1, "baz boop" },
        { 2, kDefaultValue },
        { 3, "hello=world" }
    };
    EXPECT_TRUE(result);
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

} // namespace ahoy
