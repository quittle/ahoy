#ifndef AHOY_AHOY_PARSE_RESULT_H
#define AHOY_AHOY_PARSE_RESULT_H

#include <map>
#include <vector>

#include "ahoy/actualized_parameter.h"

namespace ahoy {

template<typename T>
class ParseResult {
  public:
    explicit ParseResult(const std::map<T, ActualizedParameter>& params,
                         const std::vector<std::string>& errors) :
            params_(params), errors_(errors), succcessful_(errors.empty()) {}

    const std::map<T, ActualizedParameter>& params() const {
        return params_;
    }

    const std::vector<std::string>& errors() const {
        return errors_;
    }

    operator bool() const {
        return succcessful_;
    }

  private:
    const std::map<T, ActualizedParameter> params_;
    const std::vector<std::string> errors_;
    const bool succcessful_;
};

} // namespace ahoy

#endif // AHOY_AHOY_PARSE_RESULT_H
