// Copyright (c) 2016 Dustin Toff
// Licensed under Apache License v2.0

#include "ahoy/parser.h"

#include <memory>
#include <vector>

#include <gtest/gtest.h>

namespace {

const char kProgram[] = "./program";
const char kValue[] = "value";
const char kValue2[] = "value 2";

bool parse(const ahoy::Parser& parser, const std::vector<std::string>& args) {
    const uint arg_len = args.size() + 1;

    const std::unique_ptr<const char*[]> array(new const char*[arg_len]);

    array[0] = kProgram;

    for (uint i = 0; i < arg_len - 1; i++) {
        array[i + 1] = args[i].c_str();
    }

    return parser.Parse(arg_len, array.get());
}

} // namespace

namespace ahoy {

TEST(Parser, Empty) {
    const Parser p;
    EXPECT_TRUE(parse(p, {}));
    EXPECT_FALSE(parse(p, { kValue }));
}

TEST(Parser, SingleOption) {
    std::string param;

    const Parser p = Parser().withOptions(Parameter(&param));
    EXPECT_TRUE(parse(p, {}));
    EXPECT_TRUE(parse(p, { kValue }));
    EXPECT_FALSE(parse(p, { kValue, kValue }));
    EXPECT_EQ(kValue, param);
}

TEST(Parser, SingleNext) {
    std::string param;

    const Parser p = Parser().then(Parameter(&param));
    EXPECT_FALSE(parse(p, {}));
    EXPECT_TRUE(parse(p, { kValue }));
    EXPECT_FALSE(parse(p, { kValue, kValue }));
    EXPECT_EQ(kValue, param);
}

TEST(Parser, OptionAndNext) {
    std::string option;
    std::string then;

    const Parser p = Parser().withOptions(Parameter(&option)).then(Parameter(&then));
    EXPECT_FALSE(parse(p, {}));
    EXPECT_FALSE(parse(p, { kValue }));
    EXPECT_TRUE(parse(p, { kValue, kValue2 }));
    EXPECT_EQ(kValue, option);
    EXPECT_EQ(kValue2, then);
}

TEST(Parser, ProgramName) {
    std::string program_name;

    char const * const args[] = { kProgram };
    EXPECT_TRUE(Parser().Parse(1, args, &program_name));
    EXPECT_EQ(kProgram, program_name);
}

TEST(Parser, GetCurrentOptions) {
    Parser parser;
    ASSERT_EQ(0, parser.current_options().size());
    parser.withOptions({});
    ASSERT_EQ(0, parser.current_options().size());
    Parameter parameter((bool*) nullptr);
    parser.withOptions(parameter);
    const std::vector<Parameter>& parameters = parser.current_options();
    ASSERT_EQ(1, parameters.size());
    ASSERT_EQ(parameter, parameters[0]);
    ASSERT_EQ(std::vector<Parameter>({parameter}), parser.current_options());
}

} // namespace ahoy
