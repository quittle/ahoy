#ifndef AHOY_AHOY_ACTUALIZED_PARAMETER_H
#define AHOY_AHOY_ACTUALIZED_PARAMETER_H

#include <iostream>
#include <string>

namespace ahoy {

class ActualizedParameter {
  public:
    ActualizedParameter();
    ActualizedParameter(const std::string& value);
    ActualizedParameter(char const * const value);
    ActualizedParameter(const bool value);

    std::string AsString() const;
    int AsInt() const;
    unsigned int AsUInt() const;
    long AsLong() const;
    unsigned long AsULong() const;
    bool AsBool() const;

    bool operator==(const ActualizedParameter& rhs) const;
    bool operator!=(const ActualizedParameter& rhs) const;

  private:
    const std::string value_;
};

} // namespace ahoy

std::ostream& operator<<(std::ostream& os, const ahoy::ActualizedParameter& param);

#endif // AHOY_AHOY_ACTUALIZED_PARAMETER_H
