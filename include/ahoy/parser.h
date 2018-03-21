// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#ifndef AHOY_AHOY_PARSER_H
#define AHOY_AHOY_PARSER_H

#include <map>
#include <string>
#include <vector>

#include "ahoy/newline.h"
#include "ahoy/options.h"
#include "ahoy/options2.h"
#include "ahoy/internal/formal_parameter.h"
#include "ahoy/internal/static_assert_helper.h"
#include "ahoy/internal/type.h"

// "Private" macro to generate named and positional parameters for each variable type. This is less
// risky and more manageble than copy-pasting with slight modifications. These are the public
// methods on the class to add paramaters before parsing. The generated methods take a pointer to
// the variable to store the values in along with a vararg of options associated with the parameter
// such as name, description, and if the parameter is required or not.
#define _AHOY_PARSER_ADD_PARAM(type_name, INTERNAL_TYPE_NAME) \
    template<class ...Options> \
    Parser& AddNamedParam(type_name* pointer, Options... options) { \
        ::ahoy::internal::OptionChecker<Options...>{}; \
        ::ahoy::internal::FormalParameter fp; \
        fp.type(INTERNAL_TYPE_NAME); \
        BuildFormalNamedParameter(&fp, options...); \
        named_pointers_list_.push_back({pointer}); \
        named_pointers_.insert({pointer, fp}); \
        return *this; \
    } \
    \
    template<class ...Options> \
    Parser& AddPositionalParam(type_name* pointer, Options... options) { \
        ::ahoy::internal::FormalParameter fp; \
        fp.type(INTERNAL_TYPE_NAME); \
        BuildFormalPositionalParameter(&fp, options...); \
        positional_pointers_list_.push_back({pointer}); \
        positional_pointers_.insert({pointer, fp}); \
        return *this; \
    }

// "Private" macro to generate the internal unpacking of options for parameters. This generates
// unpacking of an option for both named and positional parameters
#define _AHOY_PARSER_BUILD_FORMAL_PARAMETER(OptionClass, formal_parameter_value) \
    _AHOY_PARSER_BUILD_FORMAL_NAMED_PARAMETER(OptionClass, formal_parameter_value) \
    _AHOY_PARSER_BUILD_FORMAL_POSITIONAL_PARAMETER(OptionClass, formal_parameter_value)

// "Private" macro to generate the internal unpacking of options for parameters. This generates
// unpacking of an option for named parameters
#define _AHOY_PARSER_BUILD_FORMAL_NAMED_PARAMETER(OptionClass, formal_parameter_value) \
    template<class ...Options> \
    void BuildFormalNamedParameter(internal::FormalParameter* fp, const OptionClass& option_value, \
                                   Options... options) { \
        fp->formal_parameter_value(option_value.get()); \
        BuildFormalNamedParameter(fp, options...); \
    }

// "Private" macro to generate the internal unpacking of options for parameters. This generates
// unpacking of an option for positional parameters
#define _AHOY_PARSER_BUILD_FORMAL_POSITIONAL_PARAMETER(OptionClass, formal_parameter_value) \
    template<class ...Options> \
    void BuildFormalPositionalParameter(internal::FormalParameter* fp, \
                                        const OptionClass& option_value, \
                                        Options... options) { \
        fp->formal_parameter_value(option_value.get()); \
        BuildFormalPositionalParameter(fp, options...); \
    }

namespace ahoy {
namespace internal {

// Validates the varargs of options
template <typename... Options>
class OptionChecker {
    // Check that Options does not contain both ahoy::Flag and ahoy::Required
    static_assert(!does_contain_type_2<Flag, Required, Options...>::value,
                  "Named parameter may not be both an ahoy::Flag and ahoy::Required.");
};

} // namespace internal

// Parses arguments from the main() function
class Parser {
  public:
    Parser();
    virtual ~Parser();

    template <typename... P>
    Parser& withOptions(P&&... parameters) {
        return withOptions({ std::forward<Parameter>(parameters)... });
    }

    template <typename... P>
    Parser& then(P&&... parameters) {
        return then({ std::forward<Parameter>(parameters)... });
    }

    // These macros generate methods in the forms
    //
    // template<Options... options>
    // Parser& AddNamedParam(bool*, Options...);
    //
    // template<Options... options>
    // Parser& AddPositionalParam(bool*, Options...);
    //
    // The first argument is a pointer to memory to hold the parameters value once parsed. The type
    // will enforce characteristics of arguments when parsing. e.g. a parameter whose type is int*
    // will cause Parse to return false if "/tmp/file" is passed by the user because it cannot be
    // interpreted as an int.
    //
    // The rest of the arguments are options to define the type of the parameter.
    // e.g. parser.AddPostionalParam(&param, ahoy::Required(), ahoy::Name("Param Name"));
    //
    // They return a reference to the original Parser for method chaining.

    _AHOY_PARSER_ADD_PARAM(bool, internal::Type::BOOL)
    _AHOY_PARSER_ADD_PARAM(char, internal::Type::CHAR)
    _AHOY_PARSER_ADD_PARAM(unsigned char, internal::Type::U_CHAR)
    _AHOY_PARSER_ADD_PARAM(short, internal::Type::SHORT)
    _AHOY_PARSER_ADD_PARAM(unsigned short, internal::Type::U_SHORT)
    _AHOY_PARSER_ADD_PARAM(int, internal::Type::INT)
    _AHOY_PARSER_ADD_PARAM(unsigned int, internal::Type::U_INT)
    _AHOY_PARSER_ADD_PARAM(long, internal::Type::LONG)
    _AHOY_PARSER_ADD_PARAM(unsigned long, internal::Type::U_LONG)
    _AHOY_PARSER_ADD_PARAM(long long, internal::Type::LONG_LONG)
    _AHOY_PARSER_ADD_PARAM(unsigned long long, internal::Type::U_LONG_LONG)
    _AHOY_PARSER_ADD_PARAM(float, internal::Type::FLOAT)
    _AHOY_PARSER_ADD_PARAM(double, internal::Type::DOUBLE)
    _AHOY_PARSER_ADD_PARAM(long double, internal::Type::LONG_DOUBLE)
    _AHOY_PARSER_ADD_PARAM(std::string, internal::Type::STRING)

    // Generates a help message for the parameters assigned to the parser
    // newline - Determines the type of line ending to use to between lines of the message
    std::string HelpMessage(const Newline newline = Newline::AUTO) const;

    // Parses the arguments from the main() function, updating the values of parameters passed in
    // via the add param methods. Returns true if the arguments were parsed successfully and false
    // if some constraints could not be met, e.g. missing required parameters or parameters that are
    // unsigned but are passed a negative number
    bool Parse2(const int argc, char const * const argv[]) const;

  private:
    Parser& withOptions(const std::vector<Parameter>& paramters);
    Parser& then(const std::vector<Parameter>& paramters);

    // Builds formal parameters from vararg options
    _AHOY_PARSER_BUILD_FORMAL_PARAMETER(Name, name)
    _AHOY_PARSER_BUILD_FORMAL_PARAMETER(Description, description)
    _AHOY_PARSER_BUILD_FORMAL_PARAMETER(Required, required)

    // Builds formal parameters from vararg, named-parameter-specific options
    _AHOY_PARSER_BUILD_FORMAL_NAMED_PARAMETER(ShortForms, short_forms)
    _AHOY_PARSER_BUILD_FORMAL_NAMED_PARAMETER(LongForms, long_forms)
    _AHOY_PARSER_BUILD_FORMAL_NAMED_PARAMETER(Flag, flag)

    // Final method to deal with recursively built formal parameters
    void BuildFormalNamedParameter(internal::FormalParameter* /* fp */) {}
    void BuildFormalPositionalParameter(internal::FormalParameter* /* fp */) {}

    std::vector<Parameter> current_options_;
    std::vector<Parameter> next_options_;

    // All the named pointers, including flags
    std::vector<void*> named_pointers_list_;
    std::map<void*, internal::FormalParameter> named_pointers_;
    // All the positional pointers, the vector is to keep order
    std::vector<void*> positional_pointers_list_;
    std::map<void*, internal::FormalParameter> positional_pointers_;
};

} // namespace ahoy

#endif // AHOY_AHOY_PARSER_H
