#ifndef AHOY_AHOY_PARSER_H
#define AHOY_AHOY_PARSER_H

#include <algorithm>
#include <cstring>
#include <map>
#include <set>
#include <vector>

#include "ahoy/arg.h"
#include "ahoy/param.h"
#include "ahoy/parse_result.h"

// TODO: Remove
#include <iostream>

namespace ahoy {

template<typename T>
class Parser {
  public:
    explicit Parser<T>(const std::map<T, Arg>& args) : args_(args) {}

    // Parses the args from a standard main function
    ParseResult<T> Parse(const int argc, char const * const argv[]) const {
        char const * const kEquals = "=";
        const size_t kEqualsLen = strlen(kEquals);
        const char kHyphen = '-';

        const char kStateInit = 0;
        const char kStateSeekingKey = 0;
        const char kStateSeekingValue = 1;
        const char kArgSizeInit = -1;
        const char kArgSizeShort = 0;
        const char kArgSizeLong = 1;

        std::map<T, Param> params;
        std::vector<std::string> errors;

        // Skip the first arg, which is executable name
        for (int i = 1; i < argc; i++) {
            char const * const arg = argv[i];
            const size_t arg_len = strlen(arg);
            if (arg[0] != kHyphen) { // Regular argument
                // Not supported yet
                errors.emplace_back(std::string("Regular args found: ") + arg);
                continue;
            } else { // - or -- argument
                const char* dehyphenated_arg = nullptr;
                char arg_size = kArgSizeInit;

                // Account for hyphens
                if (arg[1] != kHyphen) { // Short argument
                    dehyphenated_arg = arg + 1;
                    arg_size = kArgSizeShort;
                } else if (arg[2] != kHyphen) { // Long argument
                    dehyphenated_arg = arg + 2;
                    arg_size = kArgSizeLong;
                } else {
                    // Too many hyphens `---foo`
                    errors.emplace_back(std::string("Too many hyphens for arg: ") + arg);
                    continue;
                }

                std::string key, value;
                char const * const equalsLoc = strstr(dehyphenated_arg, kEquals);
                if (equalsLoc == NULL) {
                    std::cout << "No equals found" << std::endl;
                    i++;
                    if (i >= argc) {
                        // Ran out of args to parse when looking for a value
                        errors.emplace_back(std::string("Ran out of arguments to parse but was looking for the value of ") + arg);
                        continue;
                    }
                    key = std::string(dehyphenated_arg);
                    value = argv[i];
                } else {
                    key = std::string(dehyphenated_arg, equalsLoc - dehyphenated_arg);
                    value = std::string(equalsLoc + kEqualsLen);
                }

                std::cout << "Value: " << value << std::endl;

                bool arg_was_expected = false;
                for (const auto& entry : args_) {
                    const T& type_key = entry.first;
                    const Arg& arg = entry.second;
                    if ((arg.short_forms().find(key) != arg.short_forms().end() && arg_size == kArgSizeShort) ||
                            (arg.long_forms().find(key) != arg.long_forms().end() && arg_size == kArgSizeLong)) {
                        params.insert({type_key, value});
                        arg_was_expected = true;
                        break;
                    }
                }

                if (!arg_was_expected) {
                    errors.emplace_back(std::string("Arg was unexpected because it has an unexpected name or was expected to be - or -- but was other. Name: \"") + key + + "\" Value: \"" + value + "\"");
                    continue;
                }
            }
        }

        for (const auto& entry : args_) {
            if (params.find(entry.first) == params.end()) {
                if (entry.second.required()) {
                    errors.emplace_back(std::string("Missing required field: [") + Join(entry.second.short_forms()) + "] [" + Join(entry.second.long_forms()) + "]");
                    continue;
                } else {
                    params.insert({entry.first, entry.second.default_value()});
                }
            }
        }

        for (const auto& entry : params) {
            std::cout << entry.first << " " << entry.second.AsString() << std::endl;
        }

        return ParseResult<T>(params, errors);
    }

  private:
    const std::map<T, Arg> args_;

    std::string Join(std::set<std::string> set) const {
        return std::accumulate(set.begin(), set.end(), std::string(),
                [](const std::string& prev, const std::string& item) {
                    return std::string(prev.empty() ? "" : ", ") + item;
                });
    }
};

} // namespace ahoy

#endif // AHOY_AHOY_PARSER_H
