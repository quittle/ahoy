#ifndef AHOY_AHOY_ARG_H
#define AHOY_AHOY_ARG_H

#include <set>
#include <string>

namespace ahoy {

class Arg {
  public:
    // Arg or Required Switch
    Arg(const std::set<std::string>& short_forms,
            const std::set<std::string>& long_forms,
            const std::string& description,
            const bool is_flag = false);
    // Non-required Switch. If this just relied on the implicit constructor, then `char*` would
    // implicitly have the boolean operator called instead of the std::string constructor
    Arg(const std::set<std::string>& short_forms,
            const std::set<std::string>& long_forms,
            const std::string& description,
            char const * const default_value);
    // Non-required Switch
    Arg(const std::set<std::string>& short_forms,
            const std::set<std::string>& long_forms,
            const std::string& description,
            const std::string& default_value);
    virtual ~Arg();

    // Returns true if |form| is in it's set of short forms
    bool HasShortForm(const std::string& form) const;
    // Returns true if |form| is in it's set of long forms
    bool HasLongForm(const std::string& form) const;
    // Returns true if the arg is a flag or false if it has a string default value
    bool IsFlag() const;

    const std::set<std::string>& short_forms() const;
    const std::set<std::string>& long_forms() const;
    const std::string& description() const;
    bool IsRequired() const;
    // Returns nullptr if the arg was a flag
    const std::string* default_value() const;

  private:
    const std::set<std::string> short_forms_;
    const std::set<std::string> long_forms_;
    const std::string description_;
    const bool required_;
    const std::string default_value_;
    // This should be true if the Arg is a boolean flag
    const bool is_flag_;
};

} // namespace ahoy

#endif // AHOY_AHOY_ARG_H
