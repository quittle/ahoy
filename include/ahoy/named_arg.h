#ifndef AHOY_AHOY_NAMED_ARG_H
#define AHOY_AHOY_NAMED_ARG_H

#include <string>

namespace ahoy {

class NamedArg {
  public:
    NamedArg(const std::string& name,
             const std::string& description);
    virtual ~NamedArg();

    const std::string& name() const;
    const std::string& description() const;

  private:
    const std::string name_;
    const std::string description_;
};

} // namespace ahoy

#endif // AHOY_AHOY_NAMED_ARG_H
