#include "ahoy/param.h"

#include <algorithm>
#include <string>
#include <unordered_set>

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
const std::unordered_set<std::string> kBoolsTrue = {
    "true", "TRUE", "t", "y", "Yes"
};
const std::unordered_set<std::string> kBoolsFalse = {
    "false", "FALSE", "F", "NO"
};

} // namespace

namespace ahoy {

TEST(Param, AsString) {
    EXPECT_EQ(kString, Param(kString).AsString());
    EXPECT_EQ(kInt, Param(kInt).AsString());
    EXPECT_EQ(kLong, Param(kLong).AsString());
    EXPECT_EQ(kBoolIntTrue, Param(kBoolIntTrue).AsString());
    EXPECT_EQ(kBoolIntFalse, Param(kBoolIntFalse).AsString());
    for (const std::string& bool_str : kBoolsTrue) {
        EXPECT_EQ(bool_str, Param(bool_str).AsString());
    }
    for (const std::string& bool_str : kBoolsFalse) {
        EXPECT_EQ(bool_str, Param(bool_str).AsString());
    }
}

TEST(Param, AsInt) {
    EXPECT_THROW(Param(kString).AsInt(), std::invalid_argument);
    EXPECT_EQ(kIntValue, Param(kInt).AsInt());
    EXPECT_EQ((int) kLongValue, Param(kLong).AsInt());
    EXPECT_EQ(1, Param(kBoolIntTrue).AsInt());
    EXPECT_EQ(0, Param(kBoolIntFalse).AsInt());
    for (const std::string& bool_str : kBoolsTrue) {
        EXPECT_THROW(Param(bool_str).AsInt(), std::invalid_argument);
    }
    for (const std::string& bool_str : kBoolsFalse) {
        EXPECT_EQ(bool_str, Param(bool_str).AsString());
    }
}

TEST(Param, AsLong) {
    EXPECT_THROW(Param(kString).AsLong(), std::invalid_argument);
    EXPECT_EQ((long) kIntValue, Param(kInt).AsLong());
    EXPECT_EQ(kLongValue, Param(kLong).AsLong());
    EXPECT_EQ(1L, Param(kBoolIntTrue).AsLong());
    EXPECT_EQ(0L, Param(kBoolIntFalse).AsLong());
    for (const std::string& bool_str : kBoolsTrue) {
        EXPECT_THROW(Param(bool_str).AsLong(), std::invalid_argument);
    }
    for (const std::string& bool_str : kBoolsFalse) {
        EXPECT_EQ(bool_str, Param(bool_str).AsString());
    }
}

TEST(Param, AsBool) {
    EXPECT_THROW(Param(kString).AsBool(), std::invalid_argument);
    EXPECT_THROW(Param(kInt).AsBool(), std::invalid_argument);
    EXPECT_THROW(Param(kLong).AsBool(), std::invalid_argument);
    EXPECT_EQ(true, Param(kBoolIntTrue).AsBool());
    EXPECT_EQ(false, Param(kBoolIntFalse).AsBool());
    for (const std::string& bool_str : kBoolsTrue) {
        EXPECT_EQ(true, Param(bool_str).AsBool());
    }
    for (const std::string& bool_str : kBoolsFalse) {
        EXPECT_EQ(false, Param(bool_str).AsBool());
    }
}

} // namespace ahoy