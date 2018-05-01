// Copyright (c) 2016, 2018 Dustin Toff
// Licensed under Apache License v2.0

#include "ahoy/parser.h"

#include <vector>

namespace ahoy {

Parser::Parser() : current_options_(), next_options_() {}

Parser::~Parser() {}

Parser& Parser::withOptions(const std::vector<Parameter>& paramters) {
    current_options_ = paramters;
    return *this;
}

Parser& Parser::then(const std::vector<Parameter>& paramters) {
    next_options_ = paramters;
    return *this;
}

bool Parser::Parse(const int argc, char const * const argv[], std::string* program_name) const {
    // Alternate holder to store the program name
    std::string* ptr = program_name;
    std::string alternate;
    if (ptr == nullptr) {
        ptr = &alternate;
    }

    const std::vector<std::string> args(argv, argv + argc);
    Parameter root(ptr);
    root.withOptions(current_options_).then(next_options_);

    return root.consume(args) == static_cast<internal::size_t>(args.size());
}

const std::vector<Parameter>& Parser::current_options() const {
    return current_options_;
}

const std::vector<Parameter>& Parser::next_options() const {
    return next_options_;
}

} // namespace ahoy
