// Copyright (c) 2016, 2018 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/parser.h"

#include <list>
#include <locale>
#include <sstream>
#include <set>
#include <utility>

#include "ahoy/internal/assign.h"

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

    std::list<std::string> args(argv, argv + argc);
    Parameter root(ptr);
    root.withOptions(current_options_).then(next_options_);
    const bool success = root.consume(args);
    return success && args.size() == 0;
}

} // namespace ahoy
