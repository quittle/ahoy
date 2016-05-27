#ifndef AHOY_AHOY_PARAM_H
#define AHOY_AHOY_PARAM_H

#include <string>

namespace ahoy {

class Param {
  public:
    Param();
    Param(const std::string& value);
    Param(char const * const value);
    //Param(const Param&) = default;

    std::string AsString() const;
    int AsInt() const;
    long AsLong() const;
    bool AsBool() const;

    Param& operator=(const Param&) = default;
    //Param& operator=(Param&&) = default;
    bool operator==(const Param& rhs) const;
    bool operator!=(const Param& rhs) const;

  private:
    const std::string value_;
};

} // namespace ahoy

#endif // AHOY_AHOY_PARAM_H
