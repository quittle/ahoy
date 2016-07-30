// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/parser.h"

#include <gtest/gtest.h>

namespace {

const char kProgram[] = "./program";
const char kName[] = "name";
const char kDescription[] = "description";
const std::set<std::string> kShortForms = { "short" };
const std::set<std::string> kLongForms = { "long" };

} // namespace

namespace ahoy {

TEST(Parser, Help) {
    int int_param;
    std::string string_param;
    short short_flag;
    unsigned char positional_uchar;
    long long required_positional_long_long;
    bool positional_bool;

    Parser p;
    p.AddNamedParam(&int_param, Name("Int Param"), Description("Int Param description"),
                                ShortForms({"ip"}), LongForms({"int-param"}));
    p.AddNamedParam(&string_param, Name("String Param"), ShortForms({"sp", "string-param"}),
                                   LongForms({}), Required());
    p.AddNamedParam(&short_flag, Name("Short Flag"), Flag(), LongForms({"short-flag"}));
    p.AddPositionalParam(&positional_uchar, Description("Postional UChar description"));
    p.AddPositionalParam(&required_positional_long_long, Required());
    p.AddPositionalParam(&positional_bool, Name("Positional Bool"));
    EXPECT_EQ(
        "-ip --int-param [Integer] - Int Param - Int Param description\n"
        "-sp -string-param (Required) [String] - String Param\n"
        "--short-flag (Flag) [Short] - Short Flag\n"
        "[Unsigned Character] - Postional UChar description\n"
        "(Required) [Long Long]\n"
        "[Boolean] - Positional Bool\n",
        p.HelpMessage(Newline::POSIX)
    );
}

TEST(Parser, ParamTypesAdded) {
    bool param_1;
    char param_2;
    short param_3;
    int param_4;
    long param_5;
    unsigned long param_6;
    std::string param_7;

    Parser().AddNamedParam(&param_1)
            .AddNamedParam(&param_2)
            .AddNamedParam(&param_3)
            .AddNamedParam(&param_4)
            .AddNamedParam(&param_5)
            .AddNamedParam(&param_6)
            .AddNamedParam(&param_7);
}

TEST(Parser, OptionsAdded) {
    bool param;
    Parser().AddNamedParam(&param, Name(kName), Description(kDescription), ShortForms(kShortForms),
                                    LongForms(kLongForms), Required());
    Parser().AddNamedParam(&param, Name(kName), Description(kDescription), ShortForms(kShortForms),
                                    LongForms(kLongForms), Flag());
    Parser().AddPositionalParam(&param, Name(kName), Description(kDescription));
}

TEST(Parser, Basic) {
    bool bool_named_param = false;
    char char_named_param = -1;
    unsigned char uchar_flag = -1;
    short short_named_param = -1;
    unsigned short ushort_pos_param = -1;
    int int_flag = -1;
    unsigned int uint_flag = -1;
    long long_named_param = -1;
    unsigned long ulong_pos_param = -1;
    long long long_long_flag = -1;
    unsigned long long u_long_long_named_param = -1;
    float float_pos_param = -1;
    double double_flag = -1;
    long double long_double_named_param = -1;
    std::string string_named_param;

    Parser p;
    p.AddNamedParam(&bool_named_param, ShortForms({"arg-1"}));
    p.AddNamedParam(&char_named_param, LongForms({"char-named-param"}));
    p.AddNamedParam(&uchar_flag, ShortForms({"uchar"}), Flag());
    p.AddNamedParam(&short_named_param, LongForms({"short-named-param"}));
    p.AddPositionalParam(&ushort_pos_param);
    p.AddNamedParam(&int_flag, ShortForms({"2"}), Flag());
    p.AddNamedParam(&uint_flag, ShortForms({"uint"}), Flag());
    p.AddNamedParam(&long_named_param, LongForms({"long-arg"}));
    p.AddPositionalParam(&ulong_pos_param);
    p.AddNamedParam(&long_long_flag, ShortForms({"ll"}), Flag());
    p.AddNamedParam(&u_long_long_named_param, ShortForms({"ull"}));
    p.AddPositionalParam(&float_pos_param);
    p.AddNamedParam(&double_flag, LongForms({"double-flag"}), Flag());
    p.AddNamedParam(&long_double_named_param, LongForms({"long-double-flag"}));
    p.AddNamedParam(&string_named_param, LongForms({"string"}));

    char const * const args[19] = {
        kProgram,
        "-arg-1=true",
        "--char-named-param=4",
        "-uchar",
        "--short-named-param=-300",
        "300",
        "-2",
        "-uint",
        "--long-arg", "345",
        "998877665544",
        "-ll",
        "-ull=12345678901234567890",
        "1.234",
        "--double-flag",
        "--long-double-flag", "22.44",
        "--string", "string value"
    };
    EXPECT_TRUE(p.Parse(19, args));

    EXPECT_EQ(true, bool_named_param);
    EXPECT_EQ('4', char_named_param);
    EXPECT_EQ(1, uchar_flag);
    EXPECT_EQ(-300, short_named_param);
    EXPECT_EQ(300, ushort_pos_param);
    EXPECT_EQ(1, int_flag);
    EXPECT_EQ(1, uint_flag);
    EXPECT_EQ(345, long_named_param);
    EXPECT_EQ(998877665544LL, ulong_pos_param);
    EXPECT_EQ(12345678901234567890ULL, u_long_long_named_param);
    EXPECT_EQ(1.234f, float_pos_param);
    EXPECT_EQ(1, double_flag);
    EXPECT_EQ(22.44L, long_double_named_param);
    EXPECT_EQ("string value", string_named_param);
}

TEST(Parser, NamedParamOrder) {
    int param_1 = -1, param_2 = -1, param_3 = -1;
    Parser p;
    p.AddNamedParam(&param_1, ShortForms({"1"}));
    p.AddNamedParam(&param_2, ShortForms({"2"}));
    p.AddNamedParam(&param_3, ShortForms({"3"}));
    char const * const args[4] =  { kProgram, "-3=3", "-1=1", "-2=2" };
    EXPECT_TRUE(p.Parse(4, args));

    EXPECT_EQ(1, param_1);
    EXPECT_EQ(2, param_2);
    EXPECT_EQ(3, param_3);
}

TEST(Parser, PositionalParamOrder) {
    int param_1 = -1, param_2 = -1, param_3 = -1;
    bool param_4 = false;
    Parser p;
    p.AddPositionalParam(&param_1);
    p.AddPositionalParam(&param_2);
    p.AddPositionalParam(&param_3);
    p.AddNamedParam(&param_4, ShortForms({"4"}), Flag());
    char const * const args[5] =  { kProgram, "-4", "1", "2", "3" };
    EXPECT_TRUE(p.Parse(5, args));

    EXPECT_EQ(1, param_1);
    EXPECT_EQ(2, param_2);
    EXPECT_EQ(3, param_3);
    EXPECT_EQ(true, param_4);
}

TEST(Parser, MultipleFlags) {
    int param_1 = -1, param_2 = -1;

    Parser p;
    p.AddNamedParam(&param_1, ShortForms({"p1", "1"}), LongForms({"param1", "param-1", "1"}));
    p.AddNamedParam(&param_2, ShortForms({"p2", "2"}), LongForms({"param2", "param-2", "2"}));

    char const * const args[11] = {
        kProgram, "-1=1", "--param-2=2", "--param1=11", "--param2=22", "--1=111", "-p1=1111",
        "-p2=222", "--1=11111", "--2=2222", "-2=22222" };
    EXPECT_TRUE(p.Parse(11, args));

    EXPECT_EQ(11111, param_1);
    EXPECT_EQ(22222, param_2);
}

TEST(Parser, UnusedParameter) {
    int param_1 = -1, param_2 = -1, flag_3 = -1;

    Parser p;
    p.AddNamedParam(&param_1, ShortForms({"1"}));
    p.AddNamedParam(&param_2, ShortForms({"2"}));
    p.AddNamedParam(&flag_3, ShortForms({"3"}), Flag());

    char const * const args[2] = { kProgram, "-1=1" };

    EXPECT_TRUE(p.Parse(2, args));

    EXPECT_EQ(1, param_1);
    EXPECT_EQ(-1, param_2);
    EXPECT_EQ(0, flag_3);
}

TEST(Parser, Required_NotMet) {
    int param_1 = -1, param_2 = -1;

    Parser p;
    p.AddNamedParam(&param_1, ShortForms({"1"}), Required());
    p.AddNamedParam(&param_2, ShortForms({"2"}), Required());

    char const * const args[3] = { kProgram, "-1=1", "-1=2" };

    EXPECT_FALSE(p.Parse(3, args));

    EXPECT_EQ(2, param_1);
    EXPECT_EQ(-1, param_2);
}

TEST(Parser, Required_Met) {
    int param_1 = -1, param_2 = -1;

    Parser p;
    p.AddNamedParam(&param_1, ShortForms({"1"}), Required());
    p.AddNamedParam(&param_2, ShortForms({"2"}), Required());

    char const * const args[3] = { kProgram, "-1=1", "-2=2" };

    EXPECT_TRUE(p.Parse(3, args));

    EXPECT_EQ(1, param_1);
    EXPECT_EQ(2, param_2);
}

TEST(Parser, NotRequired) {
    int param_1 = -1, param_2 = -1;

    Parser p;
    p.AddNamedParam(&param_1, ShortForms({"1"}), Required(false));
    p.AddNamedParam(&param_2, ShortForms({"2"}));

    char const * const args[1] = { kProgram };
    EXPECT_TRUE(p.Parse(1, args));

    EXPECT_EQ(-1, param_1);
    EXPECT_EQ(-1, param_2);
}

TEST(Parser, FlagRequiredChecking) {
    int param;
    Parser().AddNamedParam(&param, Description(kDescription));
    Parser().AddNamedParam(&param, Description(kDescription), Description(kDescription));
    Parser().AddNamedParam(&param, Description(kDescription), Name(kName));
    Parser().AddNamedParam(&param, Description(kDescription), Flag());
    Parser().AddNamedParam(&param, Description(kDescription), Required());
    Parser().AddNamedParam(&param, Required(), Description(kDescription), Required());
    Parser().AddNamedParam(&param, Flag(), Description(kDescription), Flag(), Flag());
}

TEST(Parser, Parse2_ExtraParams) {
    char const * const args[5] = { kProgram, "--arg1=value1", "-arg2", "2", "positional" };
    Parser p;
    EXPECT_FALSE(p.Parse(5, args));
}

} // namespace ahoy
