#include "ahoy/parse_result.h"

#include "ahoy/actualized_parameter.h"

#include <gtest/gtest.h>

namespace {

const char kErrorMessage[] = "error message";
const char kParam[] = "param";

} // namespace

namespace ahoy {

TEST(ParseResult, Empty) {
    ParseResult<int> r({}, {});
    EXPECT_TRUE(r.params().empty());
    EXPECT_TRUE(r.errors().empty());
    EXPECT_TRUE(r);
}

TEST(ParseResult, Errors) {
    ParseResult<int> r({}, { kErrorMessage });
    EXPECT_TRUE(r.params().empty());
    const std::vector<std::string> expected_errors = { kErrorMessage };
    EXPECT_EQ(expected_errors, r.errors());
    EXPECT_FALSE(r);
}

TEST(ParseResult, Params) {
    ParseResult<int> r({ { 0, kParam } }, {});
    const std::map<int, ActualizedParameter> expected_params = { { 0, kParam } };
    EXPECT_EQ(expected_params, r.params());
    EXPECT_TRUE(r.errors().empty());
    EXPECT_TRUE(r);
}

TEST(ParseResult, ParamsAndErrors) {
    ParseResult<int> r({ { 0, kParam } }, { kErrorMessage});
    const std::map<int, ActualizedParameter> expected_params = { { 0, kParam } };
    EXPECT_EQ(expected_params, r.params());
    const std::vector<std::string> expected_errors = { kErrorMessage };
    EXPECT_EQ(expected_errors, r.errors());
    EXPECT_FALSE(r);
}

} // namespace ahoy
