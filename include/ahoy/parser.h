// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#ifndef AHOY_AHOY_PARSER_H
#define AHOY_AHOY_PARSER_H

#include <map>
#include <string>
#include <vector>

#include "ahoy/newline.h"
#include "ahoy/options.h"
#include "ahoy/parameter.h"
#include "ahoy/internal/formal_parameter.h"
#include "ahoy/internal/static_assert_helper.h"
#include "ahoy/internal/type.h"

namespace ahoy {

// Parses arguments from the main() function
class Parser {
  public:
    Parser();
    virtual ~Parser();

    Parser& withOptions(const std::vector<Parameter>& parameters);

    template <typename... P>
    Parser& withOptions(P&&... parameters) {
        return withOptions({ std::forward<Parameter>(parameters)... });
    }

    Parser& then(const std::vector<Parameter>& parameters);

    template <typename... P>
    Parser& then(P&&... parameters) {
        return then({ std::forward<Parameter>(parameters)... });
    }

    // Parses the arguments from the main() function, updating the values of parameters passed in
    // via the add param methods. Returns true if the arguments were parsed successfully and false
    // if some constraints could not be met, e.g. missing required parameters or parameters that are
    // unsigned but are passed a negative number
    bool Parse(const int argc, char const * const argv[]) const;

  private:
    std::vector<Parameter> current_options_;
    std::vector<Parameter> next_options_;
};

} // namespace ahoy

#endif // AHOY_AHOY_PARSER_H
