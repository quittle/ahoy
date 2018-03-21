// Copyright (c) 2016, 2018 Dustin Doloff
// Licensed under Apache License v2.0

#ifndef AHOY_AHOY_PARAMETER_H
#define AHOY_AHOY_PARAMETER_H

#include <list>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "ahoy/options.h"
#include "ahoy/internal/assign.h"
#include "ahoy/internal/formal_parameter.h"
#include "ahoy/internal/static_assert_helper.h"

#define _AHOY_PARAMETER_STATIC_ASSERT_NOT_BOTH(a, b) \
    static_assert(!ahoy::internal::does_contain_type_2<b, a, Options...>::value, \
                  "Parameter may not be both an #a and #b.")

// Validates the varargs of options
template <typename... Options>
class OptionChecker {
    _AHOY_PARAMETER_STATIC_ASSERT_NOT_BOTH(ahoy::Flag, ahoy::Required);
    _AHOY_PARAMETER_STATIC_ASSERT_NOT_BOTH(ahoy::ShortForms, ahoy::Flag);
    _AHOY_PARAMETER_STATIC_ASSERT_NOT_BOTH(ahoy::LongForms, ahoy::Flag);
};

// These macros generate methods in the form
//
// template<Options... options>
// Parser& BuildFormalNamedParameter(bool*, Options...);
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
#define _AHOY_PARSER_BUILD_FORMAL_PARAMETER(OptionClass, formal_parameter_value) \
    template<class ...Options> \
    static void BuildFormalNamedParameter(internal::FormalParameter* fp, \
                                          const OptionClass& option_value, \
                                          Options... options) { \
        fp->formal_parameter_value(option_value.get()); \
        BuildFormalNamedParameter(fp, options...); \
    }

#define _AHOY_PARAMETER_CTR(PointerType, InternalType) \
    template<class ...Options> \
    explicit Parameter(PointerType* storage, Options... options) { \
        OptionChecker<Options...>{}; \
        fp_.type(InternalType); \
        BuildFormalNamedParameter(&fp_, options...); \
        storage_ = storage; \
    }

namespace ahoy {

class Parameter {
   public:
    _AHOY_PARAMETER_CTR(bool, internal::Type::BOOL);
    _AHOY_PARAMETER_CTR(char, internal::Type::CHAR);
    _AHOY_PARAMETER_CTR(unsigned char, internal::Type::U_CHAR);
    _AHOY_PARAMETER_CTR(short, internal::Type::SHORT);
    _AHOY_PARAMETER_CTR(unsigned short, internal::Type::U_SHORT);
    _AHOY_PARAMETER_CTR(int, internal::Type::INT);
    _AHOY_PARAMETER_CTR(unsigned int, internal::Type::U_INT);
    _AHOY_PARAMETER_CTR(long, internal::Type::LONG);
    _AHOY_PARAMETER_CTR(unsigned long, internal::Type::U_LONG);
    _AHOY_PARAMETER_CTR(long long, internal::Type::LONG_LONG);
    _AHOY_PARAMETER_CTR(unsigned long long, internal::Type::U_LONG_LONG);
    _AHOY_PARAMETER_CTR(float, internal::Type::FLOAT);
    _AHOY_PARAMETER_CTR(double, internal::Type::DOUBLE);
    _AHOY_PARAMETER_CTR(long double, internal::Type::LONG_DOUBLE);
    _AHOY_PARAMETER_CTR(std::string, internal::Type::STRING);

    virtual ~Parameter();

    // Sets optional parameters that follow the current one
    Parameter& withOptions(const std::vector<Parameter>& parameters);

    // Sets optional parameters that follow the current one
    template <typename... P>
    Parameter& withOptions(P&&... parameters) {
        return withOptions({ std::forward<Parameter>(parameters)... });
    }

    // Sets required parameters that must follow the current one
    Parameter& then(const std::vector<Parameter>& parameters);

    // Sets required parameters that must follow the current one
    template <typename... P>
    Parameter& then(P&&... parameters) {
        return then({ std::forward<Parameter>(parameters)... });
    }

    // Greedily consumes arguments and sets this and dependent parameter values, returning true if
    // succesful. If unsuccessful, the value stored by the pointer passed in may be modified.
    bool consume(std::list<std::string>& args) const;

  private:
    _AHOY_PARSER_BUILD_FORMAL_PARAMETER(ahoy::LongForms, long_forms)
    _AHOY_PARSER_BUILD_FORMAL_PARAMETER(ahoy::ShortForms, short_forms)
    _AHOY_PARSER_BUILD_FORMAL_PARAMETER(ahoy::Name, name)
    _AHOY_PARSER_BUILD_FORMAL_PARAMETER(ahoy::Description, description)
    _AHOY_PARSER_BUILD_FORMAL_PARAMETER(ahoy::Required, required)
    _AHOY_PARSER_BUILD_FORMAL_PARAMETER(ahoy::Flag, flag)
    static void BuildFormalNamedParameter(internal::FormalParameter*);

    void* storage_;
    internal::FormalParameter fp_;

    std::vector<Parameter> current_options_;
    std::vector<Parameter> next_options_;
};

} // namespace ahoy

#endif // AHOY_AHOY_PARAMETER_H
