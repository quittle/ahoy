#ifndef AHOY_AHOY_PARSER_H
#define AHOY_AHOY_PARSER_H

#include <algorithm>
#include <cstring>
#include <map>
#include <set>
#include <sstream>
#include <vector>

#include "ahoy/arg.h"
#include "ahoy/newline.h"
#include "ahoy/param.h"
#include "ahoy/parse_result.h"

namespace ahoy {

template<typename T>
class Parser {
  public:
    // Builds a parser with the given |args|. If the args are invalid, the Parser will be falsey and
    // should be tested like so:
    // Parser parser = Parser<int>::Create({...});
    // if (!parser) {
    //   // Print error message and quit.
    // }
    // Note: If this parser is invalid, then it is almost definitely a programming error, not a user
    // error so this should value should be deterministic at runtime.
    static Parser<T> Create(const std::map<T, Arg>& args) {
        std::set<std::string> short_forms;
        std::set<std::string> long_forms;
        for (const auto& arg : args) {
            const std::set<std::string> args_short_forms = arg.second.short_forms();
            for (const std::string& form : args_short_forms) {
                if (short_forms.find(form) != short_forms.end()) {
                    return Parser<T>(args, false);
                }
                short_forms.insert(form);
            }
            const std::set<std::string> args_long_forms = arg.second.long_forms();
            for (const std::string& form : args_long_forms) {
                if (long_forms.find(form) != long_forms.end()) {
                    return Parser<T>(args, false);
                }
                long_forms.insert(form);
            }
        }
        return Parser<T>(args, true);
    }

    // Generates a help message for the args assigned to the parser
    // newline - Determines the type of line ending to use to between lines of the message
    std::string HelpMessage(Newline newline = Newline::AUTO) const {
        const std::string line_ending = NewlineToString(newline);

        std::stringstream ss;
        for (const auto& entry : args_) {
            const Arg arg = entry.second;
            bool first = true;

            // Add short forms
            for (const std::string& form : arg.short_forms()) {
                if (first) {
                    first = false;
                } else {
                    ss << " ";
                }

                ss << "-" << form;
            }

            // Add long forms
            for (const std::string& form : arg.long_forms()) {
                if (first) {
                    first = false;
                } else {
                    ss << " ";
                }

                ss << "--" << form;
            }

            // Add required / default value
            ss << " (";
            if (arg.IsFlag()) {
                ss << "Flag";
            } else if (arg.required()) {
                ss  << "Required";
            } else {
                ss << "Defaults to " << *arg.default_value();
            }
            ss << ")";

            // Add description
            ss << " - " << arg.description();

            // Add line ending
            ss << line_ending;
        }
        return ss.str();
    }

    // Parses the args from a standard main function.
    // Note: Duplicate fields will use first found field
    ParseResult<T> Parse(const int argc, char const * const argv[]) const {
        char const * const kEquals = "=";
        const size_t kEqualsLen = strlen(kEquals);
        const char kHyphen = '-';

        std::map<T, Param> params;
        std::vector<std::string> errors;

        if (!valid_) {
            errors.emplace_back("Use of an invalid parser. Program failed to build a valid "
                                "parser.");
        }

        if (argc < 1) {
            errors.emplace_back(std::string() +
                    "Missing executable from args. Args length: " + std::to_string(argc));
        }

        // Skip the first arg, which is executable name
        for (int i = 1; i < argc; i++) {
            char const * const arg = argv[i];
            if (arg[0] != kHyphen) { // Regular argument
                // Not supported yet
                errors.emplace_back(std::string() + "Regular args found: " + arg);
                continue;
            } else { // - or -- argument
                const char* dehyphenated_arg = nullptr;
                char arg_size = kArgSizeInvalid;

                // Account for hyphens
                if (arg[1] != kHyphen) { // Short argument
                    dehyphenated_arg = arg + 1;
                    arg_size = kArgSizeShort;
                } else if (arg[2] != kHyphen) { // Long argument
                    dehyphenated_arg = arg + 2;
                    arg_size = kArgSizeLong;
                } else {
                    // Too many hyphens `---foo`
                    errors.emplace_back(std::string() + "Too many hyphens for arg: " + arg);
                    continue;
                }

                std::string key, value;
                char const * const equalsLoc = strstr(dehyphenated_arg, kEquals);
                if (equalsLoc == NULL) {
                    key = std::string(dehyphenated_arg);
                    T args_key;
                    if (Find(key, arg_size, args_key) && args_.at(args_key).IsFlag()) {
                        if (params.find(args_key) == params.end()) {
                            params.insert({args_key, true});
                        } else {
                            errors.emplace_back(std::string() + "Duplicate of arg found. Name: " +
                                    key);
                        }
                        continue;
                    }
                    i++;
                    if (i >= argc) {
                        // Ran out of args to parse when looking for a value
                        errors.emplace_back(std::string() +
                                "Ran out of arguments to parse but was looking for the value of " +
                                arg);
                        continue;
                    }
                    value = argv[i];
                } else {
                    key = std::string(dehyphenated_arg, equalsLoc - dehyphenated_arg);
                    value = std::string(equalsLoc + kEqualsLen);
                }

                bool arg_was_expected = false;
                for (const auto& entry : args_) {
                    const T& type_key = entry.first;
                    const Arg& arg = entry.second;
                    if ((arg_size == kArgSizeShort && arg.HasShortForm(key)) ||
                            (arg_size == kArgSizeLong && arg.HasLongForm(key))) {
                        if (params.find(type_key) == params.end()) {
                            params.insert({type_key, value});
                        } else {
                            errors.emplace_back(std::string() + "Duplicate of arg found. Name: " +
                                    key);
                        }
                        arg_was_expected = true;
                        break;
                    }
                }

                if (!arg_was_expected) {
                    errors.emplace_back(std::string() +
                            "Arg was unexpected because it has an unexpected name or was expected "
                            "to be - or -- but was other. Name: \"" + key +
                            "\" Value: \"" + value + "\"");
                    continue;
                }
            }
        }

        for (const auto& entry : args_) {
            if (params.find(entry.first) == params.end()) {
                if (entry.second.required()) {
                    errors.emplace_back(std::string() + "Missing required field: [" +
                            Join(entry.second.short_forms()) + "] [" +
                            Join(entry.second.long_forms()) + "]");
                    continue;
                } else if (entry.second.IsFlag()) {
                    params.insert({entry.first, false});
                } else {
                    params.insert({entry.first, *entry.second.default_value()});
                }
            }
        }

        return ParseResult<T>(params, errors);
    }

    // Returns the validity of the Parser. If false, the parser was initialized incorrectly and
    // should not be used in production code. This value should most likely be deterministic at
    // runtime.
    operator bool() const {
        return valid_;
    }

  private:
    static const char kArgSizeInvalid = -1;
    static const char kArgSizeShort = 0;
    static const char kArgSizeLong = 1;

    const std::map<T, Arg> args_;
    const bool valid_;


    explicit Parser<T>(const std::map<T, Arg>& args, const bool valid) :
            args_(args), valid_(valid) {}

    // Finds the first arg that matches |form| and |arg_size|
    // Returns true if found and false if not. If true is returned, then ret will be updated with
    // the key for the arg.
    bool Find(const std::string& form, const char arg_size, T& ret) const {
        for (const auto& entry : args_) {
            const T& key = entry.first;
            const Arg& arg = entry.second;
            if ((arg_size == kArgSizeShort && arg.HasShortForm(form)) ||
                        (arg_size == kArgSizeLong && arg.HasLongForm(form))) {
                    ret = key;
                    return true;
            }
        }
        return false;
    }

    static std::string Join(std::set<std::string> set) {
        return std::accumulate(set.begin(), set.end(), std::string(),
                [](const std::string& prev, const std::string& item) {
                    return std::string(prev.empty() ? "" : ", ") + item;
                });
    }
};

} // namespace ahoy

#endif // AHOY_AHOY_PARSER_H
