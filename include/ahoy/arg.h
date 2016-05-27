#ifndef AHOY_AHOY_ARG_H
#define AHOY_AHOY_ARG_H

#include <string>
#include <unordered_set>

namespace ahoy {

class Arg {
  public:
    Arg(const std::unordered_set<std::string>& short_forms,
            const std::unordered_set<std::string>& long_forms,
            const bool required,
            const std::string& description,
            const std::string& default_value);
    virtual ~Arg();

    const std::unordered_set<std::string>& short_forms() const;
    const std::unordered_set<std::string>& long_forms() const;
    bool required() const;
    const std::string& description() const;
    const std::string& default_value() const;

  private:
    const std::unordered_set<std::string> short_forms_;
    const std::unordered_set<std::string> long_forms_;
    const bool required_;
    const std::string description_;
    const std::string default_value_;
};

} // namespace ahoy

#endif // AHOY_AHOY_ARG_H
