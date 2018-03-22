// Copyright (c) 2016, 2018 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/parser.h"

#include <list>
#include <locale>
#include <sstream>
#include <set>
#include <utility>

#include "ahoy/internal/arg_generator.h"
#include "ahoy/internal/arg_size.h"
#include "ahoy/internal/args.h"
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

bool Parser::Parse(const int argc, char const * const argv[]) const {
    std::list<std::string> args(argv, argv + argc);
    std::string program_name;
    Parameter root(&program_name);
    root.withOptions(current_options_).then(next_options_);
    const bool success = root.consume(args);
    return success && args.size() == 0;
}

} // namespace ahoy
