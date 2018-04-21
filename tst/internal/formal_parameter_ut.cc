// Copyright (c) 2016, 2018 Dustin Toff
// Licensed under Apache License v2.0

#include "ahoy/internal/formal_parameter.h"

#include <algorithm>
#include <set>
#include <string>

#include "ahoy/internal/type.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace {

const char kName[] = "name";
const char kDescription[] = "description";
const std::set<std::string> kForms = { "a", "forms" };
const std::set<std::string> kShortForms = { "-a", "-forms" };
const std::set<std::string> kLongForms = { "--a", "--forms" };
const ahoy::internal::Type kType = ahoy::internal::Type::STRING;

const std::set<std::string> kEmptySet;

} // namespace

namespace ahoy {
namespace internal {

TEST(FormalParameter, DefaultValues) {
    FormalParameter fp;

    EXPECT_EQ("", fp.name());
    EXPECT_EQ("", fp.description());
    EXPECT_EQ(kEmptySet, fp.forms());
    EXPECT_FALSE(fp.required());
    EXPECT_FALSE(fp.flag());
    EXPECT_EQ(Type::INVALID, fp.type());
}

TEST(FormalParameter, GetSet) {
    {
        FormalParameter fp;
        EXPECT_NE(kName, fp.name());
        fp.name(kName);
        EXPECT_EQ(kName, fp.name());
    }

    {
        FormalParameter fp;
        EXPECT_NE(kDescription, fp.description());
        fp.description(kDescription);
        EXPECT_EQ(kDescription, fp.description());
    }

    {
        FormalParameter fp;
        EXPECT_NE(kForms, fp.forms());
        fp.forms(kForms);
        EXPECT_EQ(kForms, fp.forms());
    }

    {
        FormalParameter fp;
        EXPECT_NE(kShortForms, fp.forms());
        fp.short_forms(kForms);
        EXPECT_EQ(kShortForms, fp.forms());
    }

    {
        FormalParameter fp;
        EXPECT_NE(kLongForms, fp.forms());
        fp.long_forms(kForms);
        EXPECT_EQ(kLongForms, fp.forms());
    }

    {

        FormalParameter fp;
        fp.short_forms(kForms);
        fp.forms(kForms);
        fp.long_forms(kForms);

        for (const std::string form : kForms) {
            ASSERT_THAT(fp.forms(), ::testing::Contains(form));
        }

        for (const std::string form : kShortForms) {
            ASSERT_THAT(fp.forms(), ::testing::Contains(form));
        }

        for (const std::string form : kLongForms) {
            ASSERT_THAT(fp.forms(), ::testing::Contains(form));
        }

        ASSERT_EQ(fp.forms().size(), kForms.size() + kShortForms.size() + kLongForms.size());
    }

    {
        FormalParameter fp;
        EXPECT_FALSE(fp.required());
        fp.required(true);
        EXPECT_TRUE(fp.required());
    }

    {
        FormalParameter fp;
        EXPECT_FALSE(fp.flag());
        fp.flag(true);
        EXPECT_TRUE(fp.flag());
    }

    {
        FormalParameter fp;
        EXPECT_NE(kType, fp.type());
        fp.type(kType);
        EXPECT_EQ(kType, fp.type());
    }
}

TEST(FormalParameter, Equality) {
    FormalParameter fp1, fp2;
    ASSERT_EQ(fp1, fp1);
    ASSERT_EQ(fp1, fp2);

    fp1.name("name");
    ASSERT_NE(fp1, fp2);
    fp2.name("name");
    ASSERT_EQ(fp1, fp2);

    fp1.description("desc");
    fp1.short_forms({std::string("short forms")});
    fp1.long_forms({std::string("long forms")});
    fp1.required(true);
    fp1.flag(true);
    fp1.type(Type::INVALID);

    ASSERT_EQ(fp1, fp1);
    ASSERT_NE(fp1, fp2);

    fp2.description("desc");
    fp2.short_forms({std::string("short forms")});
    fp2.long_forms({std::string("long forms")});
    fp2.required(true);
    fp2.flag(true);
    fp2.type(Type::STRING);

    ASSERT_NE(fp1, fp2);

    fp1.type(Type::STRING);

    ASSERT_EQ(fp1, fp2);
}

} // namespace internal
} // namespace ahoy
