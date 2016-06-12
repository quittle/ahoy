// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/actualized_parameter.h"

#include <algorithm>
#include <climits>
#include <set>
#include <string>

#include <gtest/gtest.h>

namespace {

template <typename T>
std::string Stringify(T v) {
    std::stringstream ss;
    ss << v;
    return ss.str();
}

const char kEmpty[] = "";
const char kString[] = "string";
const std::string kInt = Stringify(INT_MAX);
const int kIntValue = INT_MAX;
const std::string kUInt = Stringify(UINT_MAX);
const unsigned int kUIntValue = UINT_MAX;
const std::string kLong = Stringify(LONG_MAX);
const long kLongValue = LONG_MAX;
const std::string kULong = Stringify(ULONG_MAX);
const unsigned long kULongValue = ULONG_MAX;
const char kBoolIntTrue[] = "1";
const char kBoolIntFalse[] = "0";
const std::set<std::string> kBoolsTrue = {
    "true", "TRUE", "t", "y", "Yes"
};
const std::set<std::string> kBoolsFalse = {
    "false", "FALSE", "F", "NO"
};

} // namespace

namespace ahoy {

TEST(ActualizedParameter, AsString) {
    EXPECT_EQ(kString, ActualizedParameter(kString).AsString());
    EXPECT_EQ(kInt, ActualizedParameter(kInt).AsString());
    EXPECT_EQ(kUInt, ActualizedParameter(kUInt).AsString());
    EXPECT_EQ(kLong, ActualizedParameter(kLong).AsString());
    EXPECT_EQ(kULong, ActualizedParameter(kULong).AsString());
    EXPECT_EQ(kBoolIntTrue, ActualizedParameter(kBoolIntTrue).AsString());
    EXPECT_EQ(kBoolIntFalse, ActualizedParameter(kBoolIntFalse).AsString());
    for (const std::string& bool_str : kBoolsTrue) {
        EXPECT_EQ(bool_str, ActualizedParameter(bool_str).AsString());
    }
    for (const std::string& bool_str : kBoolsFalse) {
        EXPECT_EQ(bool_str, ActualizedParameter(bool_str).AsString());
    }
}

TEST(ActualizedParameter, AsInt) {
    EXPECT_THROW(ActualizedParameter(kString).AsInt(), std::invalid_argument);
    EXPECT_EQ(kIntValue, ActualizedParameter(kInt).AsInt());
    EXPECT_THROW(ActualizedParameter(kUInt).AsInt(), std::out_of_range);
    EXPECT_THROW(ActualizedParameter(kLong).AsInt(), std::out_of_range);
    EXPECT_THROW(ActualizedParameter(kULong).AsInt(), std::out_of_range);
    EXPECT_EQ(1, ActualizedParameter(kBoolIntTrue).AsInt());
    EXPECT_EQ(0, ActualizedParameter(kBoolIntFalse).AsInt());
    for (const std::string& bool_str : kBoolsTrue) {
        EXPECT_THROW(ActualizedParameter(bool_str).AsInt(), std::invalid_argument);
    }
    for (const std::string& bool_str : kBoolsFalse) {
        EXPECT_THROW(ActualizedParameter(bool_str).AsInt(), std::invalid_argument);
    }
}

TEST(ActualizedParameter, AsUInt) {
    EXPECT_THROW(ActualizedParameter(kString).AsUInt(), std::invalid_argument);
    EXPECT_EQ((unsigned int) kIntValue, ActualizedParameter(kInt).AsUInt());
    EXPECT_EQ(kUIntValue, ActualizedParameter(kUInt).AsUInt());
    EXPECT_THROW(ActualizedParameter(kLong).AsUInt(), std::out_of_range);
    EXPECT_THROW(ActualizedParameter(kULong).AsUInt(), std::out_of_range);
    EXPECT_EQ(1, ActualizedParameter(kBoolIntTrue).AsUInt());
    EXPECT_EQ(0, ActualizedParameter(kBoolIntFalse).AsUInt());
    for (const std::string& bool_str : kBoolsTrue) {
        EXPECT_THROW(ActualizedParameter(bool_str).AsUInt(), std::invalid_argument);
    }
    for (const std::string& bool_str : kBoolsFalse) {
        EXPECT_THROW(ActualizedParameter(bool_str).AsUInt(), std::invalid_argument);
    }
}

TEST(ActualizedParameter, AsLong) {
    EXPECT_THROW(ActualizedParameter(kString).AsLong(), std::invalid_argument);
    EXPECT_EQ((long) kIntValue, ActualizedParameter(kInt).AsLong());
    EXPECT_EQ((long) kUIntValue, ActualizedParameter(kUInt).AsLong());
    EXPECT_EQ(kLongValue, ActualizedParameter(kLong).AsLong());
    EXPECT_THROW(ActualizedParameter(kULong).AsLong(), std::out_of_range);
    EXPECT_EQ(1L, ActualizedParameter(kBoolIntTrue).AsLong());
    EXPECT_EQ(0L, ActualizedParameter(kBoolIntFalse).AsLong());
    for (const std::string& bool_str : kBoolsTrue) {
        EXPECT_THROW(ActualizedParameter(bool_str).AsLong(), std::invalid_argument);
    }
    for (const std::string& bool_str : kBoolsFalse) {
        EXPECT_THROW(ActualizedParameter(bool_str).AsLong(), std::invalid_argument);
    }
}

TEST(ActualizedParameter, AsULong) {
    EXPECT_THROW(ActualizedParameter(kString).AsULong(), std::invalid_argument);
    EXPECT_EQ((unsigned long) kIntValue, ActualizedParameter(kInt).AsULong());
    EXPECT_EQ((unsigned long) kUIntValue, ActualizedParameter(kUInt).AsULong());
    EXPECT_EQ((unsigned long) kLongValue, ActualizedParameter(kLong).AsULong());
    EXPECT_EQ(kULongValue, ActualizedParameter(kULong).AsULong());
    EXPECT_EQ(1L, ActualizedParameter(kBoolIntTrue).AsULong());
    EXPECT_EQ(0L, ActualizedParameter(kBoolIntFalse).AsULong());
    for (const std::string& bool_str : kBoolsTrue) {
        EXPECT_THROW(ActualizedParameter(bool_str).AsULong(), std::invalid_argument);
    }
    for (const std::string& bool_str : kBoolsFalse) {
        EXPECT_THROW(ActualizedParameter(bool_str).AsULong(), std::invalid_argument);
    }
}

TEST(ActualizedParameter, AsBool) {
    EXPECT_THROW(ActualizedParameter(kString).AsBool(), std::invalid_argument);
    EXPECT_THROW(ActualizedParameter(kInt).AsBool(), std::invalid_argument);
    EXPECT_THROW(ActualizedParameter(kUInt).AsBool(), std::invalid_argument);
    EXPECT_THROW(ActualizedParameter(kLong).AsBool(), std::invalid_argument);
    EXPECT_THROW(ActualizedParameter(kULong).AsBool(), std::invalid_argument);
    EXPECT_EQ(true, ActualizedParameter(kBoolIntTrue).AsBool());
    EXPECT_EQ(false, ActualizedParameter(kBoolIntFalse).AsBool());
    for (const std::string& bool_str : kBoolsTrue) {
        EXPECT_EQ(true, ActualizedParameter(bool_str).AsBool());
    }
    for (const std::string& bool_str : kBoolsFalse) {
        EXPECT_EQ(false, ActualizedParameter(bool_str).AsBool());
    }
}

} // namespace ahoy
