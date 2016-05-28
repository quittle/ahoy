#ifndef AHOY_AHOY_ARG_H
#define AHOY_AHOY_ARG_H

#include <set>
#include <string>

namespace ahoy {

class Arg {
  public:
    Arg(const std::set<std::string>& short_forms,
            const std::set<std::string>& long_forms,
            const bool required,
            const std::string& description,
            const std::string& default_value);
    virtual ~Arg();

    const std::set<std::string>& short_forms() const;
    const std::set<std::string>& long_forms() const;
    bool required() const;
    const std::string& description() const;
    const std::string& default_value() const;

  private:
    const std::set<std::string> short_forms_;
    const std::set<std::string> long_forms_;
    const bool required_;
    const std::string description_;
    const std::string default_value_;
};

} // namespace ahoy

#endif // AHOY_AHOY_ARG_H
