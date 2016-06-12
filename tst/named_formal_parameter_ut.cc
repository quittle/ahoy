// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/named_formal_parameter.h"

#include <gtest/gtest.h>

namespace {

const char kName[] = "name";
const char kDescription[] = "description";

} // namespace

namespace ahoy {

class NamedFormalParameterTest : public ::testing::Test {
  public:
    NamedFormalParameterTest() : named_param_(NamedFormalParameter(kName, kDescription)) {}

  protected:
    const NamedFormalParameter named_param_;
};

TEST_F(NamedFormalParameterTest, Name) {
    EXPECT_EQ(kName, named_param_.name());
}

TEST_F(NamedFormalParameterTest, Description) {
    EXPECT_EQ(kDescription, named_param_.description());
}

} // namespace ahoy
