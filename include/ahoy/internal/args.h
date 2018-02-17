// Copyright (c) 2016, 2018 Dustin Doloff
// Licensed under Apache License v2.0

#ifndef AHOY_AHOY_INTERNAL_ARGS_H
#define AHOY_AHOY_INTERNAL_ARGS_H

#include <string>

#include "ahoy/internal/arg_size.h"

namespace ahoy {
namespace internal {

// Representation of an argument passed in and defined by its name, without a value
// Example: ./prog --flag-arg
class FlagArg {
  public:
    // Static invalid value to use as a null-like return value
    static const FlagArg INVALID;

    FlagArg();
    explicit FlagArg(const ArgSize& size, const std::string& name);
    virtual ~FlagArg();

    const ArgSize& size() const;
    void size(const ArgSize size);

    const std::string& name() const;
    void name(const std::string& name);

    bool operator==(const FlagArg& other) const;
    bool operator!=(const FlagArg& other) const;

  private:
    ArgSize size_;
    std::string name_;
};

// Representation of an argument passed in and defined by its name with a value
// Example: ./prog --named-arg="value"
class NamedArg : public FlagArg {
  public:
    // Static invalid value to use as a null-like return value
    static const NamedArg INVALID;

    NamedArg();
    explicit NamedArg(const ArgSize& size, const std::string& name, const std::string& value,
                      const bool single_part);
    virtual ~NamedArg();

    const std::string& value() const;
    void value(const std::string& value);

    bool single_part() const;
    void single_part(const bool single_part);

    bool operator==(const NamedArg& other) const;
    bool operator!=(const NamedArg& other) const;

  private:
    std::string value_;
    bool single_part_;
};

// Representation of an argument passed in and defined by its position
// Example: ./prog "Positional Argument"
class PositionalArg {
  public:
    // Static invalid value to use as a null-like return value
    static const PositionalArg INVALID;

    PositionalArg();
    explicit PositionalArg(const std::string& value);
    virtual ~PositionalArg();

    const std::string& value() const;
    void value(const std::string& value);

    bool operator==(const PositionalArg& other) const;
    bool operator!=(const PositionalArg& other) const;

  private:
    std::string value_;
};

} // namespace internal
} // namespace ahoy

#endif // AHOY_AHOY_INTERNAL_ARGS_H
