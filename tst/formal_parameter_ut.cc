// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/formal_parameter.h"

#include <set>
#include <string>

#include "ahoy/type.h"

#include <gtest/gtest.h>

namespace {

const char kName[] = "name";
const char kDescription[] = "description";
const std::set<std::string> kShortForms = { "short", "forms" };
const std::set<std::string> kLongForms = { "long", "forms" };
const ahoy::internal::Type kType = ahoy::internal::Type::STRING;

const std::set<std::string> kEmptySet;

} // namespace

namespace ahoy {
namespace internal {

TEST(FormalParameter, DefaultValues) {
    FormalParameter fp;

    EXPECT_EQ("", fp.name());
    EXPECT_EQ("", fp.description());
    EXPECT_EQ(kEmptySet, fp.short_forms());
    EXPECT_EQ(kEmptySet, fp.long_forms());
    EXPECT_FALSE(fp.required());
    EXPECT_FALSE(fp.flag());
    EXPECT_EQ(Type::INVALID, fp.type());
}

TEST(FormalParameter, GetSet) {
    FormalParameter fp;

    EXPECT_NE(kName, fp.name());
    fp.name(kName);
    EXPECT_EQ(kName, fp.name());

    EXPECT_NE(kDescription, fp.description());
    fp.description(kDescription);
    EXPECT_EQ(kDescription, fp.description());

    EXPECT_NE(kShortForms, fp.short_forms());
    fp.short_forms(kShortForms);
    EXPECT_EQ(kShortForms, fp.short_forms());

    EXPECT_NE(kLongForms, fp.long_forms());
    fp.long_forms(kLongForms);
    EXPECT_EQ(kLongForms, fp.long_forms());

    EXPECT_FALSE(fp.required());
    fp.required(true);
    EXPECT_TRUE(fp.required());

    EXPECT_FALSE(fp.flag());
    fp.flag(true);
    EXPECT_TRUE(fp.flag());

    EXPECT_NE(kType, fp.type());
    fp.type(kType);
    EXPECT_EQ(kType, fp.type());
}

} // namespace internal
} // namespace ahoy
