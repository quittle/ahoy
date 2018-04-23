// Copyright (c) 2016, 2018 Dustin Toff
// Licensed under Apache License v2.0

#ifndef AHOY_AHOY_PARSER_H
#define AHOY_AHOY_PARSER_H

#include <string>
#include <utility>
#include <vector>

#include "ahoy/parameter.h"

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
    bool Parse(const int argc, char const * const argv[],
                std::string* program_name = nullptr) const;

    // Gets a reference to the current options vector
    const std::vector<Parameter>& current_options() const;

    // Gets a reference to the next options vector
    const std::vector<Parameter>& next_options() const;

  private:
    std::vector<Parameter> current_options_;
    std::vector<Parameter> next_options_;
};

} // namespace ahoy

#endif // AHOY_AHOY_PARSER_H
