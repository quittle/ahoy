#include "ahoy/actualized_parameter.h"

#include <algorithm>
#include <set>
#include <string>

#include <gtest/gtest.h>

namespace {

const char kEmpty[] = "";
const char kString[] = "string";
const char kInt[] = "123";
const int kIntValue = 123;
const char kLong []= "2147483647";
const long kLongValue = 2147483647L;
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
    EXPECT_EQ(kLong, ActualizedParameter(kLong).AsString());
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
    EXPECT_EQ((int) kLongValue, ActualizedParameter(kLong).AsInt());
    EXPECT_EQ(1, ActualizedParameter(kBoolIntTrue).AsInt());
    EXPECT_EQ(0, ActualizedParameter(kBoolIntFalse).AsInt());
    for (const std::string& bool_str : kBoolsTrue) {
        EXPECT_THROW(ActualizedParameter(bool_str).AsInt(), std::invalid_argument);
    }
    for (const std::string& bool_str : kBoolsFalse) {
        EXPECT_EQ(bool_str, ActualizedParameter(bool_str).AsString());
    }
}

TEST(ActualizedParameter, AsLong) {
    EXPECT_THROW(ActualizedParameter(kString).AsLong(), std::invalid_argument);
    EXPECT_EQ((long) kIntValue, ActualizedParameter(kInt).AsLong());
    EXPECT_EQ(kLongValue, ActualizedParameter(kLong).AsLong());
    EXPECT_EQ(1L, ActualizedParameter(kBoolIntTrue).AsLong());
    EXPECT_EQ(0L, ActualizedParameter(kBoolIntFalse).AsLong());
    for (const std::string& bool_str : kBoolsTrue) {
        EXPECT_THROW(ActualizedParameter(bool_str).AsLong(), std::invalid_argument);
    }
    for (const std::string& bool_str : kBoolsFalse) {
        EXPECT_EQ(bool_str, ActualizedParameter(bool_str).AsString());
    }
}

TEST(ActualizedParameter, AsBool) {
    EXPECT_THROW(ActualizedParameter(kString).AsBool(), std::invalid_argument);
    EXPECT_THROW(ActualizedParameter(kInt).AsBool(), std::invalid_argument);
    EXPECT_THROW(ActualizedParameter(kLong).AsBool(), std::invalid_argument);
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
