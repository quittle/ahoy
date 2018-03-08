// Copyright (c) 2016 Dustin Doloff
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

    return parser.Parse2(arg_len, array.get());
}

} // namespace

namespace ahoy {

TEST(Parser2, Empty) {
    const Parser p;
    EXPECT_TRUE(parse(p, {}));
    EXPECT_FALSE(parse(p, { kValue }));
}

TEST(Parser2, SingleOption) {
    std::string param;

    const Parser p = Parser().withOptions(Option2(&param));
    EXPECT_TRUE(parse(p, {}));
    EXPECT_TRUE(parse(p, { kValue }));
    EXPECT_FALSE(parse(p, { kValue, kValue }));
    EXPECT_EQ(kValue, param);
}

TEST(Parser2, SingleNext) {
    std::string param;

    const Parser p = Parser().then(Option2(&param));
    EXPECT_FALSE(parse(p, {}));
    EXPECT_TRUE(parse(p, { kValue }));
    EXPECT_FALSE(parse(p, { kValue, kValue }));
    EXPECT_EQ(kValue, param);
}

TEST(Parser2, OptionAndNext) {
    std::string option;
    std::string then;

    const Parser p = Parser().withOptions(Option2(&option)).then(Option2(&then));
    EXPECT_FALSE(parse(p, {}));
    EXPECT_FALSE(parse(p, { kValue }));
    EXPECT_TRUE(parse(p, { kValue, kValue2 }));
    EXPECT_EQ(kValue, option);
    EXPECT_EQ(kValue2, then);
}

} // namespace ahoy
