// Copyright (c) 2018 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/internal/assign.h"

#include <gtest/gtest.h>

#define EXPECT_ASSIGN_FAILURE(assignmentFunc, value) \
    EXPECT_FALSE(assignmentFunc(&value, std::string())); \
    EXPECT_FALSE(assignmentFunc(&value, std::string("  "))); \
    /* These are very long to support overflowing floats */ \
    EXPECT_FALSE(assignmentFunc(&value, std::string(10000, '1'))); \
    EXPECT_FALSE(assignmentFunc(&value, "-" + std::string(10000, '1'))); \
    EXPECT_FALSE(assignmentFunc(&value, std::string("abcd"))); \
    EXPECT_FALSE(assignmentFunc(&value, std::string("1,2"))); \
    EXPECT_FALSE(assignmentFunc(&value, std::string("1 2"))); \
    EXPECT_FALSE(assignmentFunc(&value, std::string("ß"))); \
    EXPECT_FALSE(assignmentFunc(&value, std::string("-1.23a"))); \
    EXPECT_FALSE(assignmentFunc(&value, std::string("-1.23f"))); \
    EXPECT_FALSE(assignmentFunc(&value, std::string("1.2.3"))); \
    EXPECT_FALSE(assignmentFunc(&value, std::string("1,23")))

#define EXPECT_ASSIGN_SUCCESS(assignmentFunc, value) \
    EXPECT_TRUE(assignmentFunc(&value, std::string("1"))); \
    EXPECT_EQ(1, value); \
    EXPECT_TRUE(assignmentFunc(&value, std::string("0"))); \
    EXPECT_EQ(0, value); \
    EXPECT_TRUE(assignmentFunc(&value, false)); \
    EXPECT_EQ(0, value); \
    EXPECT_TRUE(assignmentFunc(&value, true)); \
    EXPECT_EQ(1, value); \
    EXPECT_TRUE(assignmentFunc(&value, std::string("031"))); \
    EXPECT_EQ(31, value); \
    EXPECT_TRUE(assignmentFunc(&value, true)); \
    EXPECT_EQ(1, value); \
    EXPECT_TRUE(assignmentFunc(&value, false)); \
    EXPECT_EQ(0, value)

#define EXPECT_ASSIGN_SUCCESS_REGULAR(assignmentFunc, value) \
    EXPECT_TRUE(assignmentFunc(&value, std::string("1234"))); \
    EXPECT_EQ(1234, value)

#define EXPECT_ASSIGN_SUCCESS_DECIMAL(assignmentFunc, value) \
    EXPECT_TRUE(assignmentFunc(&value, std::string("0.0"))); \
    EXPECT_EQ(0, value); \
    EXPECT_TRUE(assignmentFunc(&value, std::string("1.23"))); \
    ASSERT_FLOAT_EQ(1.23, value); \
    EXPECT_TRUE(assignmentFunc(&value, std::string("-1.23"))); \
    ASSERT_FLOAT_EQ(-1.23, value)

#define EXPECT_ASSIGN_FAILURE_UNSIGNED(assignmentFunc, value) \
    EXPECT_FALSE(assignmentFunc(&value, std::string("-1"))); \
    EXPECT_FALSE(assignmentFunc(&value, std::string("-999")))

namespace ahoy {
namespace internal {

TEST(Assign, Bool_Success) {
    bool b(false);
    EXPECT_TRUE(AssignBool(&b, std::string("true")));
    EXPECT_TRUE(b);
    EXPECT_TRUE(AssignBool(&b, std::string("FALSE")));
    EXPECT_FALSE(b);
    EXPECT_TRUE(AssignBool(&b, std::string("t")));
    EXPECT_TRUE(b);
    EXPECT_TRUE(AssignBool(&b, std::string("f")));
    EXPECT_FALSE(b);
    EXPECT_TRUE(AssignBool(&b, std::string("1")));
    EXPECT_TRUE(b);
    EXPECT_TRUE(AssignBool(&b, std::string("0")));
    EXPECT_FALSE(b);
    EXPECT_TRUE(AssignBool(&b, std::string("Yes")));
    EXPECT_TRUE(b);
    EXPECT_TRUE(AssignBool(&b, std::string("no")));
    EXPECT_FALSE(b);
    EXPECT_TRUE(AssignBool(&b, std::string("Y")));
    EXPECT_TRUE(b);
    EXPECT_TRUE(AssignBool(&b, std::string("n")));
    EXPECT_FALSE(b);
    EXPECT_TRUE(AssignBool(&b, true));
    EXPECT_TRUE(b);
    EXPECT_TRUE(AssignBool(&b, false));
    EXPECT_FALSE(b);
}

TEST(Assign, Bool_Fail) {
    bool b;
    EXPECT_ASSIGN_FAILURE(AssignBool, b);
    EXPECT_FALSE(AssignBool(&b, std::string("99999999")));
    EXPECT_FALSE(AssignBool(&b, std::string("-1")));
    EXPECT_FALSE(AssignBool(&b, std::string("random;char123")));
}

TEST(Assign, Char_Success) {
    char c(0);
    EXPECT_TRUE(AssignChar(&c, std::string("c")));
    EXPECT_EQ('c', c);
    EXPECT_TRUE(AssignChar(&c, std::string("0")));
    EXPECT_EQ('0', c);
    EXPECT_NE(0, c);
    EXPECT_TRUE(AssignChar(&c, false));
    EXPECT_EQ(0, c);
    EXPECT_TRUE(AssignChar(&c, true));
    EXPECT_EQ(1, c);
}

TEST(Assign, Char_Failure) {
    char c;
    EXPECT_ASSIGN_FAILURE(AssignChar, c);
    EXPECT_FALSE(AssignChar(&c, std::string("12")));
    EXPECT_FALSE(AssignChar(&c, std::string("ß"))); // Eszet is outside the bounds of char
}

TEST(Assign, UChar_Success) {
    unsigned char c(0);
    EXPECT_TRUE(AssignUChar(&c, std::string("c")));
    EXPECT_EQ('c', c);
    EXPECT_TRUE(AssignUChar(&c, std::string("0")));
    EXPECT_EQ('0', c);
    EXPECT_TRUE(AssignUChar(&c, false));
    EXPECT_EQ(0, c);
    EXPECT_TRUE(AssignUChar(&c, true));
    EXPECT_EQ(1, c);
}

TEST(Assign, UChar_Failure) {
    unsigned char c;
    EXPECT_ASSIGN_FAILURE(AssignUChar, c);
    EXPECT_ASSIGN_FAILURE_UNSIGNED(AssignUChar, c);
    EXPECT_FALSE(AssignUChar(&c, std::string("12")));
}

TEST(Assign, Short_Success) {
    short s(0);
    EXPECT_ASSIGN_SUCCESS(AssignShort, s);
    EXPECT_ASSIGN_SUCCESS_REGULAR(AssignShort, s);
}

TEST(Assign, Short_Failure) {
    short s;
    EXPECT_ASSIGN_FAILURE(AssignShort, s);
    EXPECT_FALSE(AssignShort(&s, std::string("1234567890")));
}

TEST(Assign, UShort_Success) {
    unsigned short s(0);
    EXPECT_ASSIGN_SUCCESS(AssignUShort, s);
    EXPECT_ASSIGN_SUCCESS_REGULAR(AssignUShort, s);
}

TEST(Assign, UShort_Failure) {
    unsigned short s;
    EXPECT_ASSIGN_FAILURE(AssignUShort, s);
    EXPECT_ASSIGN_FAILURE_UNSIGNED(AssignUShort, s);
    EXPECT_FALSE(AssignUShort(&s, std::string("1234567890")));
}

TEST(Assign, Int_Success) {
    int i(0);
    EXPECT_ASSIGN_SUCCESS(AssignInt, i);
    EXPECT_ASSIGN_SUCCESS_REGULAR(AssignInt, i);
    EXPECT_TRUE(AssignInt(&i, std::string("1234567890")));
    EXPECT_EQ(1234567890, i);
}

TEST(Assign, Int_Failure) {
    int i;
    EXPECT_ASSIGN_FAILURE(AssignInt, i);
}

TEST(Assign, UInt_Success) {
    unsigned int i(0);
    EXPECT_ASSIGN_SUCCESS(AssignUInt, i);
    EXPECT_ASSIGN_SUCCESS_REGULAR(AssignUInt, i);
}

TEST(Assign, UInt_Failure) {
    unsigned int i;
    EXPECT_ASSIGN_FAILURE(AssignUInt, i);
    EXPECT_ASSIGN_FAILURE_UNSIGNED(AssignUInt, i);
}

TEST(Assign, Long_Success) {
    long l(0);
    EXPECT_ASSIGN_SUCCESS(AssignLong, l);
    EXPECT_ASSIGN_SUCCESS_REGULAR(AssignLong, l);
}

TEST(Assign, Long_Failure) {
    long l;
    EXPECT_ASSIGN_FAILURE(AssignLong, l);
}

TEST(Assign, ULong_Success) {
    unsigned long l(0);
    EXPECT_ASSIGN_SUCCESS(AssignULong, l);
    EXPECT_ASSIGN_SUCCESS_REGULAR(AssignULong, l);
}

TEST(Assign, ULong_Failure) {
    unsigned long l;
    EXPECT_ASSIGN_FAILURE(AssignULong, l);
    EXPECT_ASSIGN_FAILURE_UNSIGNED(AssignULong, l);
}
TEST(Assign, LongLong_Success) {
    long long ll(0);
    EXPECT_ASSIGN_SUCCESS(AssignLongLong, ll);
    EXPECT_ASSIGN_SUCCESS_REGULAR(AssignLongLong, ll);
}

TEST(Assign, LongLong_Failure) {
    long long ll;
    EXPECT_ASSIGN_FAILURE(AssignLongLong, ll);
}

TEST(Assign, ULongLong_Success) {
    unsigned long long ll(0);
    EXPECT_ASSIGN_SUCCESS(AssignULongLong, ll);
    EXPECT_ASSIGN_SUCCESS_REGULAR(AssignULongLong, ll);
}

TEST(Assign, ULongLong_Failure) {
    unsigned long long ll;
    EXPECT_ASSIGN_FAILURE(AssignULongLong, ll);
    EXPECT_ASSIGN_FAILURE_UNSIGNED(AssignULongLong, ll);
}

TEST(Assign, Float_Success) {
    float f(0);
    EXPECT_ASSIGN_SUCCESS(AssignFloat, f);
    EXPECT_ASSIGN_SUCCESS_REGULAR(AssignFloat, f);
    EXPECT_ASSIGN_SUCCESS_DECIMAL(AssignFloat, f);
}

TEST(Assign, Float_Failure) {
    float f;
    EXPECT_ASSIGN_FAILURE(AssignFloat, f);
}

TEST(Assign, Double_Success) {
    double d(0);
    EXPECT_ASSIGN_SUCCESS(AssignDouble, d);
    EXPECT_ASSIGN_SUCCESS_REGULAR(AssignDouble, d);
    EXPECT_ASSIGN_SUCCESS_DECIMAL(AssignDouble, d);
}

TEST(Assign, Double_Failure) {
    double d;
    EXPECT_ASSIGN_FAILURE(AssignDouble, d);
}

TEST(Assign, LongDouble_Success) {
    long double ld(0);
    EXPECT_ASSIGN_SUCCESS(AssignLongDouble, ld);
    EXPECT_ASSIGN_SUCCESS_REGULAR(AssignLongDouble, ld);
    EXPECT_ASSIGN_SUCCESS_DECIMAL(AssignLongDouble, ld);
}

TEST(Assign, LongDouble_Failure) {
    long double ld;
    EXPECT_ASSIGN_FAILURE(AssignLongDouble, ld);
}

TEST(Assign, String_Success) {
    std::string s;
    EXPECT_TRUE(AssignString(&s, std::string("a")));
    EXPECT_EQ("a", s);
    EXPECT_TRUE(AssignString(&s, std::string("")));
    EXPECT_EQ("", s);
    EXPECT_TRUE(AssignString(&s, std::string("1")));
    EXPECT_EQ("1", s);
    EXPECT_TRUE(AssignString(&s, std::string("abc def ghi")));
    EXPECT_EQ("abc def ghi", s);

    EXPECT_TRUE(AssignString(&s, true));
    EXPECT_EQ("true", s);
    EXPECT_TRUE(AssignString(&s, false));
    EXPECT_EQ("false", s);
}

} // namespace internal
} // namespace ahoy
