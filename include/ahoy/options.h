// Copyright (c) 2016, 2018 Dustin Doloff
// Licensed under Apache License v2.0

#ifndef AHOY_AHOY_OPTION_H
#define AHOY_AHOY_OPTION_H

#include <set>
#include <string>
#include <vector>

#include "ahoy/internal/formal_parameter.h"
#include "ahoy/internal/static_assert_helper.h"

// "Private" macro to declare and define Parser option classes
#define _AHOY_OPTIONS_OPTION_CLASS(ClassName, ValueType) \
    class ClassName : public ::ahoy::internal::Option<ValueType> { \
      public: \
        explicit ClassName(const ValueType& value): Option(value) {} \
    }

// "Private" macro to declare and define Parser option classes with a default value
#define _AHOY_OPTIONS_OPTION_CLASS_DEFAULT(ClassName, ValueType, default_value) \
    class ClassName : public ::ahoy::internal::Option<ValueType> { \
      public: \
        explicit ClassName(const ValueType& value = default_value): Option(value) {} \
    }

namespace ahoy {
namespace internal {

// Base type for Parser options
template<typename T>
class Option {
  public:
    const T& get() const {
        return value_;
    }

  protected:
    explicit Option(const T& value) : value_(value) {}

    virtual ~Option() {}

  private:
    const T value_;
};

} // namespace internal

// These are the named arguments for ahoy::Parser's add parameter methods.
// Example usage:
// ahoy::Description("My description")
// ahoy::ShortForms({"form-1", "form-2"})
// ahoy::Flag()
_AHOY_OPTIONS_OPTION_CLASS(Description, std::string); // Human readable description of the argument
_AHOY_OPTIONS_OPTION_CLASS(Name, std::string); // Human readable name of the argument
// _AHOY_OPTIONS_OPTION_CLASS_DEFAULT(Marker, std::string, ""); // Indicator for branching. Value is the same as the argument
_AHOY_OPTIONS_OPTION_CLASS(ShortForms, std::set<std::string>); // Short arguments, like -h or -v
_AHOY_OPTIONS_OPTION_CLASS(LongForms, std::set<std::string>); // Long arguments, like --help or --verbose
_AHOY_OPTIONS_OPTION_CLASS_DEFAULT(Required, bool, true); // Advances option, indicating the option must be set
_AHOY_OPTIONS_OPTION_CLASS_DEFAULT(Flag, bool, true); // Like marker, but sets the value to true if present

} // namespace ahoy

#endif // AHOY_AHOY_OPTION_H
