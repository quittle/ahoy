// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#ifndef AHOY_AHOY_NAMED_FORMAL_PARAMETER_H
#define AHOY_AHOY_NAMED_FORMAL_PARAMETER_H

#include <string>

namespace ahoy {

class NamedFormalParameter {
  public:
    NamedFormalParameter(const std::string& name, const std::string& description);
    virtual ~NamedFormalParameter();

    const std::string& name() const;
    const std::string& description() const;

  private:
    const std::string name_;
    const std::string description_;
};

} // namespace ahoy

#endif // AHOY_AHOY_NAMED_FORMAL_PARAMETER_H
