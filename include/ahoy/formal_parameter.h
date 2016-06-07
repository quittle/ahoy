#ifndef AHOY_AHOY_FORMAL_PARAMETER_H
#define AHOY_AHOY_FORMAL_PARAMETER_H

#include <set>
#include <string>

namespace ahoy {

class FormalParameter {
  public:
    // Flag or Required Switch
    // If |is_flag| is true, then this builds a flag, which is just a boolean value of if passed in
    // or not. If false, then it is a required switch with no default value.
    FormalParameter(const std::set<std::string>& short_forms,
            const std::set<std::string>& long_forms,
            const std::string& description,
            const bool is_flag = false);
    // Non-required Switch. If this just relied on the implicit constructor, then `char*` would
    // implicitly have the boolean operator called instead of the std::string constructor
    FormalParameter(const std::set<std::string>& short_forms,
            const std::set<std::string>& long_forms,
            const std::string& description,
            char const * const default_value);
    // Non-required Switch
    FormalParameter(const std::set<std::string>& short_forms,
            const std::set<std::string>& long_forms,
            const std::string& description,
            const std::string& default_value);
    virtual ~FormalParameter();

    // Returns true if |form| is in it's set of short forms
    bool HasShortForm(const std::string& form) const;
    // Returns true if |form| is in it's set of long forms
    bool HasLongForm(const std::string& form) const;
    // Returns true if the FormalParameter is a flag or false if it has a string default value
    bool IsFlag() const;

    const std::set<std::string>& short_forms() const;
    const std::set<std::string>& long_forms() const;
    const std::string& description() const;
    bool IsRequired() const;
    // Returns nullptr if the FormalParameter is a flag or required switch, neither of which have a
    // default value
    const std::string* default_value() const;

  private:
    const std::set<std::string> short_forms_;
    const std::set<std::string> long_forms_;
    const std::string description_;
    const bool required_;
    const std::string default_value_;
    // This should be true if the FormalParameter is a boolean flag
    const bool is_flag_;
};

} // namespace ahoy

#endif // AHOY_AHOY_FORMAL_PARAMETER_H
