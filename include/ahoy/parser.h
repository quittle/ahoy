#ifndef AHOY_AHOY_PARSER_H
#define AHOY_AHOY_PARSER_H

#include <cstring>
#include <set>
#include <unordered_map>

#include "ahoy/arg.h"
#include "ahoy/param.h"


#include <iostream>
namespace ahoy {

template<typename T>
class Parser {
  public:
    typedef std::unordered_map<T, Param> Parameters;

    explicit Parser<T>(const std::unordered_map<T, Arg>& args) : args_(args) {}

    // Parses the args from main into |out|. Returns true if successful or false if not
    // If unable to parse, |out| will be unmodified
    bool Parse(const int argc, char const * const argv[], Parameters& out) const {
        char const * const kEquals = "=";
        const size_t kEqualsLen = strlen(kEquals);
        const char kHyphen = '-';

        const char kStateInit = 0;
        const char kStateSeekingKey = 0;
        const char kStateSeekingValue = 1;
        const char kArgSizeInit = -1;
        const char kArgSizeShort = 0;
        const char kArgSizeLong = 1;

        Parameters ret;
        std::unordered_map<std::string, std::string> short_args;
        std::unordered_map<std::string, std::string> long_args;
        // Skip the first arg, which is executable name
        for (int i = 1; i < argc; i++) {
            char const * const arg = argv[i];
            const size_t arg_len = strlen(arg);
            if (arg[0] != kHyphen) { // Regular argument
                // Not supported yet
                std::cout << "Regular args found" << std::endl;
                return false;
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
                    std::cout << "Too many hyphens" << std::endl;
                    return false;
                }

                std::string key, value;
                char const * const equalsLoc = strstr(dehyphenated_arg, kEquals);
                if (equalsLoc == NULL) {
                    std::cout << "No equals found" << std::endl;
                    i++;
                    if (i >= argc) {
                        // Ran out of args to parse when looking for a value
                        std::cout << "Can't find equals but not enough args" << std::endl;
                        return false;
                    }
                    key = std::string(dehyphenated_arg);
                    value = argv[i];
                } else {
                    key = std::string(dehyphenated_arg, equalsLoc - dehyphenated_arg);
                    value = std::string(equalsLoc + kEqualsLen);
                }

                std::cout << "Value: " << value << std::endl;

                bool found_place = false;
                for (const auto& entry : args_) {
                    const T& type_key = entry.first;
                    const Arg& arg = entry.second;
                    if ((arg.short_forms().find(key) != arg.short_forms().end() && arg_size == kArgSizeShort) ||
                            (arg.long_forms().find(key) != arg.long_forms().end() && arg_size == kArgSizeLong)) {
                        ret.insert({type_key, Param(value)});
                        found_place = true;
                        break;
                    }
                }

                if (!found_place) {
                    std::cout << "didn't find place" << std::endl;
                    // Arg not in params or size doesn't match
                    return false;
                }
            }
        }

        for (const auto& entry : args_) {
            if (ret.find(entry.first) == ret.end()) {
                if (entry.second.required()) {
                    std::cout << "Required field missing";
                    return false;
                } else {
                    ret.insert({entry.first, Param(entry.second.default_value())});
                }
            }
        }

        for (const auto& entry : ret) {
            std::cout << entry.first << " " << entry.second.AsString() << std::endl;
        }

        out = ret;

        return true;
    }

  private:
    const std::unordered_map<T, Arg> args_;
};

} // namespace ahoy

#endif // AHOY_AHOY_PARSER_H
