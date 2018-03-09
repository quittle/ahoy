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

Parser::Parser() {}

Parser::~Parser() {}

Parser& Parser::withOptions(const std::vector<Option2>& options) {
    current_options_ = options;
    return *this;
}

Parser& Parser::then(const std::vector<Option2>& options) {
    next_options_ = options;
    return *this;
}

bool Parser::Parse2(const int argc, char const * const argv[]) const {
    std::list<std::string> args(argv + 1, argv + argc);
    Option2 root((bool*) nullptr);
    root.must_consume_ = true;
    root.withOptions(current_options_).then(next_options_);
    const bool success = root.consume(args);
    return success && args.size() == 0;
}

} // namespace ahoy
