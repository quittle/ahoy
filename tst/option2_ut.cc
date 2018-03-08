// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/options2.h"

#include <memory>
#include <vector>

#include <gtest/gtest.h>

namespace {

const char kProgram[] = "./program";
const char kValue[] = "value";
const char kValue2[] = "value 2";

} // namespace

namespace ahoy {

#define TYPE_TEST(Type) \
    do { \
        Type* t(nullptr); \
        Option2 o(t); \
    } while(false)

TEST(Option2, Types) {
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

TEST(Option2, Empty) {
    bool* p(nullptr);
    const Option2 o(p);
}

TEST(Option2, Chaining) {
    bool* p(nullptr);

    Option2 o(p);
    Option2(p)
        .then(o.then(o)
               .withOptions(o, o)
               .withOptions())
        .then()
        .withOptions(
            o.withOptions(Option2(p)),
            o.then(o));
}

} // namespace ahoy
