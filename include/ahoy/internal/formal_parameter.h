// Copyright (c) 2016, 2018 Dustin Toff
// Licensed under Apache License v2.0

#ifndef AHOY_AHOY_INTERNAL_FORMAL_PARAMETER_H
#define AHOY_AHOY_INTERNAL_FORMAL_PARAMETER_H

#include <set>
#include <string>

#include "ahoy/internal/type.h"

namespace ahoy {
namespace internal {

// A formal representation of a parameter for the program
class FormalParameter {
  public:
    FormalParameter();
    virtual ~FormalParameter();

    // The name of the parameter
    const std::string& name() const;
    void name(const std::string& name);

    // The description of the parameter
    const std::string& description() const;
    void description(const std::string& description);

    // A set of strings that represent the literal forms of the parameter and not modified for
    // platform-specific standards
    void forms(const std::set<std::string>& forms);

    // A set of strings that represent the short forms of the parameter, which may be modified for
    // platform-specific standards, like prefixing with '-' or '/' on unix or windows systems
    // Example transformations: n -> -n, name -> -name
    void short_forms(const std::set<std::string>& short_forms);

    // A set of strings that represent the long forms of the parameter, which may be modified for
    // platform-specific standards, like prefixing with '--' or '/' on unix or windows systems
    // Example transformations: name -> --name, full-name -> --full-name
    void long_forms(const std::set<std::string>& long_forms);

    const std::set<std::string>& forms() const;

    const std::string& marker() const;
    void marker(const std::string& marker);

    // If true, this parameter is required to be passed in as one of the program's arguments
    bool required() const;
    void required(const bool required);

    // If true, this parameter does not have a value, but is a flag that indicates a state
    // Examples: --help, --verbose
    bool flag() const;
    void flag(const bool flag);

    // Shorthand for having no forms
    bool is_positional() const;

    // The variable type of the parameter
    // Examples: string, int, bool
    Type type() const;
    void type(const Type type);

    bool operator ==(const FormalParameter& other) const;
    bool operator !=(const FormalParameter& other) const;

  private:
    std::string name_;
    std::string description_;
    std::set<std::string> forms_;
    std::string marker_;
    bool required_;
    bool flag_;
    Type type_;
};

} // namespace internal
} // namespace ahoy

#endif // AHOY_AHOY_INTERNAL_FORMAL_PARAMETER_H
