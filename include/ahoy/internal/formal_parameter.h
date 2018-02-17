// Copyright (c) 2016, 2018 Dustin Doloff
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

    // A set of strings that represent the short forms of the parameter
    // Example: -n, -name representing the name parameter
    const std::set<std::string>& short_forms() const;
    void short_forms(const std::set<std::string>& short_forms);

    // A set of strings that represent the long forms of the parameter
    // Example: --name, --full-name representing the name parameter
    const std::set<std::string>& long_forms() const;
    void long_forms(const std::set<std::string>& long_forms);

    // If true, this parameter is required to be passed in as one of the program's arguments
    bool required() const;
    void required(const bool required);

    // If true, this parameter does not have a value, but is a flag that indicates a state
    // Examples: --help, --verbose
    bool flag() const;
    void flag(const bool flag);

    // The variable type of the parameter
    // Examples: string, int, bool
    Type type() const;
    void type(const Type type);

  private:
    std::string name_;
    std::string description_;
    std::set<std::string> short_forms_;
    std::set<std::string> long_forms_;
    bool required_;
    bool flag_;
    Type type_;
};

} // namespace internal
} // namespace ahoy

#endif // AHOY_AHOY_INTERNAL_FORMAL_PARAMETER_H
