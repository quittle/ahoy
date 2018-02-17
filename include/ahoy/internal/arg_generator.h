// Copyright (c) 2016, 2018 Dustin Doloff
// Licensed under Apache License v2.0

#ifndef AHOY_AHOY_INTERNAL_ARG_GENERATOR_H
#define AHOY_AHOY_INTERNAL_ARG_GENERATOR_H

#include "ahoy/internal/args.h"

namespace ahoy {
namespace internal {

// Generates potential arguments from the args passed into a C++ main function. There is rarely one
// clear interpretation of the arguments
//
// Given "./prog --name John", there are numerous possible interpretations
// --name could be a flag and John a positional argument
// --name could be a named argument with a value of John
// Both --name and John could be positional arguments.
//
// It's up to the consumer of ArgGenerator to look at the possiblities and tell the generator how to
// proceed using the Advance methods
class ArgGenerator {
  public:
    // Helper struct representing the result of analysis of the current index in the argument.
    struct Result {
        Result() : flag_arg(FlagArg::INVALID),
                   named_arg(NamedArg::INVALID),
                   positional_arg(PositionalArg::INVALID) {}

        FlagArg flag_arg;
        NamedArg named_arg;
        PositionalArg positional_arg;
    };

    // Takes the arguments from the C++ main function
    ArgGenerator(const int argc, char const * const * const argv);

    // Looks at the current index in the arguments and returns possible interpretations of the
    // arguments
    Result Peek() const;

    // Advance the inner index by interpretting the current argument as being a flag
    void AdvanceFlagArg();

    // Advance the inner index by interpretting the current argument as being a named arg
    void AdvanceNamedArg();

    // Advance the inner index by interpretting the current argument as being a positional arg
    void AdvancePositionalArg();

    // Returns true if there are more possible arguments to Peek at and Advnace past
    bool Available() const;

  private:
    const int argc_;
    char const * const * const argv_;

    // The current index of the argument being checked in argv_
    int index_;
};

} // namespace internal
} // namespace ahoy

#endif // AHOY_AHOY_INTERNAL_ARG_GENERATOR_H
