// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/arg_generator.h"
#include "ahoy/arg_size.h"

#include <string>

namespace {

const char kEquals = '=';
const char kHyphen = '-';

// Parses |arg| and sets |args_size| and |name| appropriately.  |arg| is an input parameter, while
// |arg_size| and |name| are output parameters.
// Returns true if the argument could be parsed as a switch or false if it couldn't. If it couldn't
// be parsed, the output parameters are unmodified.
bool ParseSwitch(const std::string& arg,
                 ahoy::internal::ArgSize * const arg_size,
                 std::string * const name) {
    if (arg[0] != kHyphen) {
        return false;
    }

    if (arg[1] != kHyphen) {
        *arg_size = ahoy::internal::ArgSize::SHORT;
        *name = arg.substr(1);
    } else {
        *arg_size = ahoy::internal::ArgSize::LONG;
        *name = arg.substr(2);
    }

    return true;
}

} // namespace

namespace ahoy {
namespace internal {

ArgGenerator::ArgGenerator(const int argc, char const * const * const argv) :
        argc_(argc), argv_(argv), index_(1) {}

ArgGenerator::Result ArgGenerator::Peek() const {
    Result result;

    if (!Available()) {
        return result;
    }

    const std::string cur_arg = argv_[index_];

    ArgSize size;
    std::string name;
    if (ParseSwitch(cur_arg, &size, &name)) {
        // Flag arg could potentially contain an '='
        result.flag_arg.size(size);
        result.flag_arg.name(name);

        const size_t equals_loc = name.find(kEquals);
        if (equals_loc != std::string::npos) {
            result.named_arg.size(size);
            result.named_arg.name(name.substr(0, equals_loc));
            result.named_arg.value(name.substr(equals_loc + 1));
            result.named_arg.single_part(true);
        } else if (index_ + 1 < argc_) {
            result.named_arg.size(size);
            result.named_arg.name(name);
            result.named_arg.value(argv_[index_ + 1]);
            result.named_arg.single_part(false);
        }
    }

    // As long as there's something, it could be a positional argument
    result.positional_arg.value(cur_arg);

    return result;
}

void ArgGenerator::AdvanceFlagArg() {
    if (Peek().flag_arg != FlagArg::INVALID) {
        index_ += 1;
    }
}

void ArgGenerator::AdvanceNamedArg() {
    const NamedArg arg = Peek().named_arg;
    if (arg != NamedArg::INVALID) {
        index_ += arg.single_part() ? 1 : 2;
    }
}

void ArgGenerator::AdvancePositionalArg() {
    if (Peek().positional_arg != PositionalArg::INVALID) {
        index_ += 1;
    }
}

bool ArgGenerator::Available() const {
    return index_ < argc_;
}

} // namespace internal
} // namespace ahoy
