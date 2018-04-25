// Copyright (c) 2018 Dustin Toff
// Licensed under Apache License v2.0

#include "ahoy/parameter.h"

#include <memory>
#include <vector>

#include <gtest/gtest.h>

namespace {

const char kValue[] = "value";
const char kValue2[] = "value 2";

bool consume(const ahoy::Parameter& parameter, std::list<std::string> args) {
    return parameter.consume(args);
}

} // namespace

namespace ahoy {

#define TYPE_TEST(Type) \
    do { \
        Type* t(nullptr); \
        Parameter p(t); \
    } while(false)

TEST(Parameter, Types) {
    TYPE_TEST(bool);

    TYPE_TEST(char);
    TYPE_TEST(unsigned char);
    TYPE_TEST(short);
    TYPE_TEST(unsigned short);
    TYPE_TEST(ushort);
    TYPE_TEST(int);
    TYPE_TEST(unsigned int);
    TYPE_TEST(uint);
    TYPE_TEST(long);
    TYPE_TEST(unsigned long);
    TYPE_TEST(ulong);
    TYPE_TEST(long long);
    TYPE_TEST(unsigned long long);
    TYPE_TEST(float);
    TYPE_TEST(double);
    TYPE_TEST(long double);

    TYPE_TEST(size_t);

    TYPE_TEST(std::string);
}

TEST(Parameter, Empty) {
    bool* ptr(nullptr);
    const Parameter p(ptr);
}

TEST(Parameter, Chaining) {
    bool* ptr(nullptr);

    Parameter p(ptr);
    Parameter(ptr)
        .then(p.then(p)
               .withOptions(p, p)
               .withOptions())
        .then()
        .withOptions(
            p.withOptions(Parameter(ptr)),
            p.then(p));
}

TEST(Parameter, Required) {
    std::string s1, s2;

    Parameter p1(&s1, Required());
    Parameter p2(&s2, Required());

    EXPECT_FALSE(consume(p1, {}));
    EXPECT_EQ("", s1);

    EXPECT_TRUE(consume(p1, { kValue }));
    EXPECT_EQ(kValue, s1);

    std::string root_value;
    Parameter root = Parameter(&root_value).withOptions(p1).then(p2);

    EXPECT_FALSE(consume(root, {}));
    EXPECT_FALSE(consume(root, { "a" }));
    EXPECT_FALSE(consume(root, { "a", "b" }));
    EXPECT_TRUE(consume(root, { "r", "a", "b" }));
    EXPECT_EQ("r", root_value);
    EXPECT_EQ("a", s1);
    EXPECT_EQ("b", s2);

    bool b(false);
    root.withOptions(p1, Parameter(&b, Forms({"-b"}))).then(p2);
    EXPECT_TRUE(consume(root, { "r", "a", "b" }));
    EXPECT_EQ("r", root_value);
    EXPECT_EQ("a", s1);
    EXPECT_EQ("b", s2);
    EXPECT_FALSE(b);
}

TEST(Parameter, Forms) {
    std::string value;

    const Parameter p(&value, Forms({"v"}));

    EXPECT_FALSE(consume(p, {}));
    EXPECT_EQ("", value);

    EXPECT_FALSE(consume(p, { "v" }));
    EXPECT_EQ("", value);

    EXPECT_FALSE(consume(p, { "-o" }));
    EXPECT_EQ("", value);

    EXPECT_TRUE(consume(p, { "v", kValue }));
    EXPECT_EQ(kValue, value);
}

TEST(Parameter, ShortForms) {
    std::string value;

    const Parameter p(&value, ShortForms({"v"}));

    EXPECT_FALSE(consume(p, {}));
    EXPECT_EQ("", value);

    EXPECT_FALSE(consume(p, { "-v" }));
    EXPECT_EQ("", value);

    EXPECT_FALSE(consume(p, { "-o" }));
    EXPECT_EQ("", value);

    EXPECT_TRUE(consume(p, { "-v", kValue }));
    EXPECT_EQ(kValue, value);
}

TEST(Parameter, LongForms) {
    std::string value;

    const Parameter p(&value, LongForms({"v"}));

    EXPECT_FALSE(consume(p, {}));
    EXPECT_EQ("", value);

    EXPECT_FALSE(consume(p, { "--v" }));
    EXPECT_EQ("", value);

    EXPECT_FALSE(consume(p, { "--o" }));
    EXPECT_EQ("", value);

    EXPECT_TRUE(consume(p, { "--v", kValue }));
    EXPECT_EQ(kValue, value);
}

TEST(Parameter, ConsumeEquals) {
    std::string value;

    const Parameter p(&value, ShortForms({"v"}));
    const Parameter p_weird(&value, ShortForms({"criss=cross"}));

    EXPECT_FALSE(consume(p, { "-v" }));
    EXPECT_EQ("", value);

    value = "a";
    EXPECT_TRUE(consume(p, { "-v=" }));
    EXPECT_EQ("", value);

    EXPECT_TRUE(consume(p, { "-v==a=2==" }));
    EXPECT_EQ("=a=2==", value);

    EXPECT_TRUE(consume(p_weird, { "-criss=cross=apple=sauce" }));
    EXPECT_EQ("apple=sauce", value);
}

TEST(Parameter, Flag) {
    std::string value;

    const Parameter p(&value, Flag());

    EXPECT_FALSE(consume(p, {}));
    EXPECT_EQ("", value);

    EXPECT_TRUE(consume(p, { kValue }));
    EXPECT_EQ("true", value);
}

TEST(Parameter, Literal) {
    std::string value;

    const Parameter p(&value);

    EXPECT_FALSE(consume(p, {}));
    EXPECT_EQ("", value);

    EXPECT_TRUE(consume(p, { kValue }));
    EXPECT_EQ(kValue, value);
}

TEST(Parameter, TypeMismatch) {
    int i;
    unsigned short us;
    bool b;

    EXPECT_FALSE(consume(Parameter(&i), {"a"}));
    EXPECT_FALSE(consume(Parameter(&i), {"12345678901234567890"}));
    EXPECT_FALSE(consume(Parameter(&us), {"a"}));
    EXPECT_FALSE(consume(Parameter(&us), {"-1"}));
    EXPECT_FALSE(consume(Parameter(&b), {""}));
    EXPECT_FALSE(consume(Parameter(&b), {"01"}));
}

TEST(Parameter, Equality) {
    char c, c2;
    Parameter p1(&c), p2(&c), p3(&c);
    ASSERT_EQ(p1, p1);
    ASSERT_EQ(p1, p2);
    ASSERT_NE(p1, Parameter(&c2));

    p1.withOptions({});
    ASSERT_EQ(p1, p2);

    p1.withOptions(p3);
    ASSERT_NE(p1, p2);

    p2.withOptions(p3);
    ASSERT_EQ(p1, p2);

    p1.then(p3, p3);
    ASSERT_NE(p1, p2);

    p2.then(p3, p3);
    ASSERT_EQ(p1, p2);
}

} // namespace ahoy
