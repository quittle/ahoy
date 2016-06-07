#include "ahoy/parser.h"

#include "ahoy/actualized_parameter.h"
#include "ahoy/formal_parameter.h"
#include "ahoy/newline.h"
#include "ahoy/parse_result.h"

#include <gtest/gtest.h>

namespace {

const char kExecutable[] = "./executable";
const char kDescription[] = "description";
const char kName[] = "name";
const char kDefaultValue[] = "default value";

} // namespace

namespace ahoy {

TEST(Parser, Create_Valid) {
    Parser<int> p = Parser<int>::Create({
        { 0, { {}, {}, kDescription, kDefaultValue} },
        { 1, { {"f"}, {"flag"}, kDescription} },
        { 2, { {"flag"}, {"flagflag"}, kDescription, false} }
    });
    EXPECT_TRUE(p);
}

TEST(Parser, Create_Invalid_DuplicateSwitches) {
    Parser<int> p = Parser<int>::Create({
        { 0, { {}, {}, kDescription, kDefaultValue } },
        { 1, { {"f"}, {"flag"}, kDescription } },
        { 2, { {"f"}, {"flag"}, kDescription } }
    });
    EXPECT_FALSE(p);
}

TEST(Parser, Create_Invalid_DuplicateIds) {
    Parser<int> p = Parser<int>::Create(
        {
            { 0, { {}, {}, kDescription, kDefaultValue } },
            { 1, { {"f"}, {"flag"}, kDescription } },
        }, {
            { 2, { "Misorder is fine", kDescription } },
            { 1, { "Whoops", kDescription } },
        });
    EXPECT_FALSE(p);
}

TEST(Parser, Template) {
    const FormalParameter param = { {}, {}, kDescription, kDefaultValue };
    Parser<char>::Create({ { 'c', param } });
    Parser<int>::Create({ { 0, param } });
    Parser<long>::Create({ { 0l, param } });
    Parser<std::string>::Create({ { "string", param } });
}

TEST(Parser, Help_Windows) {
    EXPECT_EQ(
        "-f -foo --foo (Defaults to bar) - Help message 1\r\n"
        "-w (Required) - Help message 2\r\n"
        "--long-arg --long-arg-alt (Defaults to true) - Help message 3\r\n"
        "--flag (Flag) - Enables the flag\r\n"
        "filename - The file to act on\r\n",
        Parser<int>::Create(
            {
                { 0, { {"f", "foo"}, {"foo"}, "Help message 1", "bar" } },
                { 1, { {"w"}, {}, "Help message 2" } },
                { 2, { {}, {"long-arg", "long-arg-alt"}, "Help message 3", "true" } },
                { 3, { {}, {"flag"}, "Enables the flag", true } }
            }, {
                { 4, { "filename", "The file to act on" } }
            }
        ).HelpMessage(Newline::WINDOWS)
    );
}

TEST(Parser, Help_Posix) {
    EXPECT_EQ(
        "-f -foo --foo (Defaults to bar) - Help message 1\n"
        "-w (Required) - Help message 2\n"
        "--long-arg --long-arg-alt (Defaults to true) - Help message 3\n"
        "--flag (Flag) - Enables the flag\n"
        "filename - The file to act on\n",
        Parser<int>::Create(
            {
                { 0, { {"f", "foo"}, {"foo"}, "Help message 1", "bar" } },
                { 1, { {"w"}, {}, "Help message 2" } },
                { 2, { {}, {"long-arg", "long-arg-alt"}, "Help message 3", "true" } },
                { 3, { {}, {"flag"}, "Enables the flag", true } }
            }, {
                { 4, { "filename", "The file to act on" } }
            }
        ).HelpMessage(Newline::POSIX)
    );
}

TEST(Parser, InvalidParser) {
    char const * const args[2] = { kExecutable, "-f=bar" };
    const Parser<int> parser = Parser<int>::Create({
                { 0, { {"f"}, {}, kDescription, kDefaultValue } },
                { 1, { {"f"}, {}, kDescription, kDefaultValue } },  // Duplicate short switch
            });
    EXPECT_FALSE(parser);
    const ParseResult<int> result = parser.Parse(2, args);
    EXPECT_FALSE(result);
    const std::map<int, ActualizedParameter> expected_params = {
                { 0, "bar" },
                { 1, kDefaultValue }
            };
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

TEST(Parser, Parse_Simple_NamedParam) {
    char const * const args[2] = { kExecutable, "/path/to/file" };
    const ParseResult<int> result = Parser<int>::Create(
        {},
        {
            { 0, { kName, kDescription } }
        }).Parse(2, args);
    EXPECT_TRUE(result);
    const std::map<int, ActualizedParameter> expected_params = { { 0, "/path/to/file" } };
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Parse_Simple_NamedParam_Invalid_UnexpectedArg_ExpectNone) {
    char const * const args[2] = { kExecutable, "/path/to/file" };
    const ParseResult<int> result = Parser<int>::Create(
        {},
        {}).Parse(2, args);
    EXPECT_FALSE(result);
    EXPECT_EQ(0, result.params().size());
    EXPECT_EQ(1, result.errors().size());
}

TEST(Parser, Parse_Simple_NamedParam_Invalid_UnexpectedArg_ExpectOne) {
    char const * const args[3] = { kExecutable, "/path/to/file", "random arg" };
    const ParseResult<int> result = Parser<int>::Create(
        {},
        {
            { 0, { kName, kDescription } }
        }).Parse(3, args);
    EXPECT_FALSE(result);
    const std::map<int, ActualizedParameter> expected_params = { { 0, "/path/to/file" } };
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(1, result.errors().size());
}

TEST(Parser, Parse_Simple_NamedParam_Invalid_MissingArg) {
    char const * const args[1] = { kExecutable };
    const ParseResult<int> result = Parser<int>::Create(
        {},
        {
            { 0, { kName, kDescription } }
        }).Parse(1, args);
    EXPECT_FALSE(result);
    EXPECT_EQ(0, result.params().size());
    EXPECT_EQ(1, result.errors().size());
}

TEST(Parser, Parse_Simple_ValidShort_Equals) {
    char const * const args[2] = { kExecutable, "-f=bar" };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, { {"f"}, {}, kDescription, kDefaultValue } }
            }).Parse(2, args);
    EXPECT_TRUE(result);
    const std::map<int, ActualizedParameter> expected_params = { { 0, "bar" } };
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Parse_Simple_ValidShort_Space) {
    char const * const args[3] = { kExecutable, "-f", "bar" };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, { {"f"}, {}, kDescription, kDefaultValue } }
            }).Parse(3, args);
    EXPECT_TRUE(result);
    const std::map<int, ActualizedParameter> expected_params = { { 0, "bar" } };
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Simple_ValidLong_Equals) {
    char const * const args[2] = { kExecutable, "--foo=bar" };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, { {}, {"foo"}, kDescription, kDefaultValue } }
            }).Parse(2, args);
    EXPECT_TRUE(result);
    const std::map<int, ActualizedParameter> expected_params = { { 0, "bar" } };
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Simple_ValidLong_Space) {
    char const * const args[3] = { kExecutable, "--foo", "bar" };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, { {}, {"foo"}, kDescription, kDefaultValue } }
            }).Parse(3, args);
    EXPECT_TRUE(result);
    const std::map<int, ActualizedParameter> expected_params = { { 0, "bar" } };
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Parse_Simple_Invalid) {
    char const * const args[2] = { kExecutable, "-f" };
    const std::map<int, ActualizedParameter> expected_defaulted_params = { { 0, kDefaultValue } };
    const ParseResult<int> both_empty_result = Parser<int>::Create({
                { 0, { {}, {}, kDescription, kDefaultValue } }
            }).Parse(2, args);
    EXPECT_FALSE(both_empty_result);
    EXPECT_EQ(expected_defaulted_params, both_empty_result.params());
    EXPECT_EQ(1, both_empty_result.errors().size());

    const ParseResult<int> short_result = Parser<int>::Create({
                { 0, { {"f"}, {}, kDescription, kDefaultValue } }
            }).Parse(2, args);
    EXPECT_FALSE(short_result);
    EXPECT_EQ(expected_defaulted_params, short_result.params());
    EXPECT_EQ(1, short_result.errors().size());

    const ParseResult<int> long_result = Parser<int>::Create({
                { 0, { {}, {"f"}, kDescription, kDefaultValue } }
            }).Parse(2, args);
    EXPECT_FALSE(long_result);
    EXPECT_EQ(expected_defaulted_params, long_result.params());
    EXPECT_EQ(1, long_result.errors().size());
}

TEST(Parser, Parse_Required) {
    char const * const args[3] = { kExecutable, "-rf", "value" };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, { {"rf"}, {"requiredField"}, kDescription } }
            }).Parse(3, args);
    EXPECT_TRUE(result);
    const std::map<int, ActualizedParameter> expected_params = { { 0, "value" } };
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Required_Missing) {
    char const * const args[1] = { kExecutable };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, { {"requiredField"}, {}, kDescription } }
            }).Parse(1, args);
    EXPECT_FALSE(result);
    EXPECT_EQ(0, result.params().size());
    EXPECT_EQ(1, result.errors().size());
}

TEST(Parser, Parse_DefaultValue) {
    char const * const args[1] = { kExecutable };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, { {"mf"}, {"missingFiled"}, kDescription, kDefaultValue } }
            }).Parse(1, args);
    const std::map<int, ActualizedParameter> expected_params = { { 0, kDefaultValue } };
    EXPECT_TRUE(result);
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Parse_Flag_Missing) {
    char const * const args[1] = { kExecutable };
    const ParseResult<int> result =
            Parser<int>::Create({ { 0, { {"f"}, {"flag"}, kDescription, true } } })
                    .Parse(1, args);
    const std::map<int, ActualizedParameter> expected_params = { { 0, false } };
    EXPECT_TRUE(result);
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Parse_Flag) {
    char const * const args[2] = { kExecutable, "--flag" };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, { {"f"}, {"flag"}, kDescription, true } }
            }).Parse(2, args);
    const std::map<int, ActualizedParameter> expected_params = { { 0, true } };
    EXPECT_TRUE(result);
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Parse_MixedSwitchesAndNamedParams) {
    char const * const args[7] =
            { kExecutable, "-flag", "arg 1", "--count", "1", "arg 2", "-f=bar" };
    const ParseResult<int> result = Parser<int>::Create(
            {
                { 0, { {}, {"count"}, kDescription } },
                { 1, { {"flag"}, {}, kDescription, true } },
                { 2, { {"f"}, {"foo"}, kDescription, "baz" } },
            }, {
                { 3, { kName, kDescription } },
                { 4, { kName, kDescription } },
            }).Parse(7, args);
    EXPECT_TRUE(result);
    const std::map<int, ActualizedParameter> expected_params = {
                { 0, "1" },
                { 1, true },
                { 2, "bar"},
                { 3, "arg 1" },
                { 4, "arg 2" }
            };
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

TEST(Parser, Parse_Duplicates) {
    char const * const args[6] =
        { kExecutable, "-f", "value", "--field=value_2", "-flag", "--flag" };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, { {"f"}, {"field"}, kDescription } },
                { 1, { {"flag"}, {"flag"}, kDescription, true } }
            }).Parse(6, args);
    EXPECT_FALSE(result);
    const std::map<int, ActualizedParameter> expected_params = { { 0, "value" }, { 1, true } };
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(2, result.errors().size());
}

TEST(Parser, Parse_Multiple) {
    char const * const args[9] =
            { kExecutable,
              "-f=foo", "--bar", "baz boop", "-enable-everything", "-hey", "hello=world",
              "/path/to/file.jpg", "100" };
    const ParseResult<int> result = Parser<int>::Create({
                { 0, { {"f"}, {"long-f"}, kDescription, kDefaultValue } },
                { 1, { {"b"}, {"bar"}, kDescription } },
                { 2, { {"m"}, {"missing"}, kDescription, kDefaultValue } },
                { 3, { {"hey"}, {"hey"}, kDescription } },
                { 4, { {"enable-everything"}, {"enable-everything"}, kDescription, true } }
            }, {
                { 5, { "file-path", kDescription } },
                { 6, { "repetitions", kDescription } },
            }).Parse(9, args);
    const std::map<int, ActualizedParameter> expected_params = {
        { 0, "foo" },
        { 1, "baz boop" },
        { 2, kDefaultValue },
        { 3, "hello=world" },
        { 4, true },
        { 5, "/path/to/file.jpg" },
        { 6, "100" }
    };
    EXPECT_TRUE(result);
    EXPECT_EQ(expected_params, result.params());
    EXPECT_EQ(0, result.errors().size());
}

} // namespace ahoy
