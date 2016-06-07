#ifndef AHOY_AHOY_PARSER_H
#define AHOY_AHOY_PARSER_H

#include <algorithm>
#include <cstring>
#include <map>
#include <set>
#include <sstream>
#include <vector>

#include "ahoy/actualized_parameter.h"
#include "ahoy/formal_parameter.h"
#include "ahoy/named_formal_parameter.h"
#include "ahoy/newline.h"
#include "ahoy/parse_result.h"

namespace ahoy {

template<typename T>
class Parser {
  public:
    // Calls created without named params
    static Parser<T> Create(const std::map<T, FormalParameter>& params) {
        return Create(params, {});
    }

    // Builds a parser with the given formal parameters. If the parameters are invalid, the Parser
    // will be falsey and should be tested like so:
    // Parser parser = Parser<int>::Create({...});
    // if (!parser) {
    //   // Print error message and quit.
    // }
    // Note: If this parser is invalid, then it is almost definitely a programming error, not a user
    // error so this should value should be deterministic at runtime.
    static Parser<T> Create(const std::map<T, FormalParameter>& params,
                            const std::map<T, NamedFormalParameter>& named_params) {
        std::map<std::string, T> short_forms;
        std::map<std::string, T> long_forms;
        std::map<std::string, T> short_flag_forms;
        std::map<std::string, T> long_flag_forms;
        for (const auto& entry : params) {
            const FormalParameter param = entry.second;
            const T id = entry.first;
            // Check if |named_params|'s and |params|'s ids overlap
            if (named_params.find(id) != named_params.end()) {
                return Parser<T>(params,
                                 short_forms, long_forms,
                                 short_flag_forms, long_flag_forms,
                                 named_params,
                                 false);
            }
            const std::set<std::string> params_short_forms = param.short_forms();
            for (const std::string& form : params_short_forms) {
                if (short_forms.find(form) != short_forms.end()) {
                    return Parser<T>(params,
                                     short_forms, long_forms,
                                     short_flag_forms, long_flag_forms,
                                     named_params,
                                     false);
                }
                short_forms.insert({form, id});
                if (param.IsFlag()) {
                    short_flag_forms.insert({form, id});
                }
            }
            const std::set<std::string> params_long_forms = param.long_forms();
            for (const std::string& form : params_long_forms) {
                if (long_forms.find(form) != long_forms.end()) {
                    return Parser<T>(params,
                                     short_forms, long_forms,
                                     short_flag_forms, long_flag_forms,
                                     named_params,
                                     false);
                }
                long_forms.insert({form, id});
                if (param.IsFlag()) {
                    long_flag_forms.insert({form, id});
                }
            }
        }
        return Parser<T>(params,
                         short_forms, long_forms,
                         short_flag_forms, long_flag_forms,
                         named_params,
                         true);
    }

    // Generates a help message for the parameters assigned to the parser
    // newline - Determines the type of line ending to use to between lines of the message
    std::string HelpMessage(Newline newline = Newline::AUTO) const {
        const std::string line_ending = NewlineToString(newline);

        std::stringstream ss;
        for (const auto& entry : params_) {
            const FormalParameter param = entry.second;

            bool first_form = true;

            // Add short forms
            for (const std::string& form : param.short_forms()) {
                if (first_form) {
                    first_form = false;
                } else {
                    ss << " ";
                }

                ss << "-" << form;
            }

            // Add long forms
            for (const std::string& form : param.long_forms()) {
                if (first_form) {
                    first_form = false;
                } else {
                    ss << " ";
                }

                ss << "--" << form;
            }

            // Add flag / required / default value
            ss << " (";
            if (param.IsFlag()) {
                ss << "Flag";
            } else if (param.IsRequired()) {
                ss  << "Required";
            } else {
                ss << "Defaults to " << *param.default_value();
            }
            ss << ")";

            // Add description
            ss << " - " << param.description();

            // Add line ending
            ss << line_ending;
        }
        for (const auto& entry : named_params_) {
            const NamedFormalParameter named_param = entry.second;
            ss << named_param.name();
            ss << " - ";
            ss << named_param.description();
            ss << line_ending;
        }
        return ss.str();
    }

    // Parses the arguments from a standard main function.
    // Note: Duplicate fields will use first found field
    ParseResult<T> Parse(const int argc, char const * const argv[]) const {
        char const * const kEquals = "=";
        const size_t kEqualsLen = strlen(kEquals);
        const char kHyphen = '-';

        std::map<T, ActualizedParameter> actualized_params;
        std::vector<std::string> errors;

        if (!valid_) {
            errors.emplace_back("Use of an invalid parser. Program failed to build a valid "
                                "parser.");
        }

        if (argc < 1) {
            errors.emplace_back(std::string() +
                    "Missing executable from params. FormalParameters length: " +
                    std::to_string(argc));
        }

        auto cur_named_param = named_params_.begin();

        // Skip the first argument, which is executable name
        for (int i = 1; i < argc; i++) {
            char const * const arg = argv[i];
            if (arg[0] != kHyphen) { // Regular argument
                if (cur_named_param == named_params_.end()) {
                    errors.emplace_back(std::string() + "Named argument provided but unexpected. "
                            "Received unexpected argument: " + arg);
                } else {
                    actualized_params.insert({cur_named_param->first, arg});
                    cur_named_param++;
                }
                continue;
            } else { // - or -- argument
                const char* dehyphenated_arg = nullptr;
                char arg_size = kFormalArgumentSizeInvalid;

                // Account for hyphens
                if (arg[1] != kHyphen) { // Short argument
                    dehyphenated_arg = arg + 1;
                    arg_size = kFormalArgumentSizeShort;
                } else if (arg[2] != kHyphen) { // Long argument
                    dehyphenated_arg = arg + 2;
                    arg_size = kFormalArgumentSizeLong;
                } else {
                    // Too many hyphens `---foo`
                    errors.emplace_back(std::string() + "Too many hyphens for argument: " + arg);
                    continue;
                }

                std::string key, value;
                char const * const equalsLoc = strstr(dehyphenated_arg, kEquals);
                if (equalsLoc != NULL) { // '=' found in |dehyphenated_arg|
                    key = std::string(dehyphenated_arg, equalsLoc - dehyphenated_arg);
                    value = std::string(equalsLoc + kEqualsLen);
                } else {
                    key = std::string(dehyphenated_arg);

                    // Look for flags, then add and continue if appropriate
                    if (arg_size == kFormalArgumentSizeShort) {
                        const auto entry = short_flag_forms_.find(key);
                        if (entry != short_flag_forms_.end()) {
                            T param_key = entry->second;
                            if (actualized_params.find(param_key) == actualized_params.end()) {
                                actualized_params.insert({param_key, true});
                            } else {
                                errors.emplace_back(std::string() + "Duplicate of short flag "
                                        "found. Name: " + key);
                            }
                            continue;
                        }
                    } else if (arg_size == kFormalArgumentSizeLong) {
                        const auto entry = long_flag_forms_.find(key);
                        if (entry != long_flag_forms_.end()) {
                            T param_key = entry->second;
                            if (actualized_params.find(param_key) == actualized_params.end()) {
                                actualized_params.insert({param_key, true});
                            } else {
                                errors.emplace_back(std::string() + "Duplicate of long flag found. "
                                        "Name: " + key);
                            }
                            continue;
                        }
                    } else {
                        // Should never happen
                        errors.emplace_back(std::string() + "Parser in invalid state, argument is "
                                "switch but of invalid size: " + arg_size);
                        continue;
                    }

                    // At this point, the current |arg| is confirmed to not be a flag so the next
                    // value must be the value for the current switch
                    i++;
                    if (i >= argc) {
                        errors.emplace_back(std::string() +
                                "Ran out of arguments to parse but was looking for the value of " +
                                arg);
                        continue;
                    }
                    value = argv[i];
                }

                // If reached, |key| and |value| should be a switch key/value pair
                T type_key;
                if (FindFormalParameter(key, arg_size, type_key)) {
                    if (actualized_params.find(type_key) == actualized_params.end()) {
                        actualized_params.insert({type_key, value});
                    } else {
                        errors.emplace_back(std::string() + "Duplicate of param found. Name: " +
                                key);
                    }
                } else {
                    errors.emplace_back(std::string() +
                            "FormalParameter was unexpected because it has an unexpected name or "
                            "was expected to be - or -- but was other. Name: \"" + key +
                            "\" Value: \"" + value + "\"");
                    continue;
                }
            }
        }

        // Make sure all switches are defaulted and required fields aren't missing
        for (const auto& entry : params_) {
            if (actualized_params.find(entry.first) == actualized_params.end()) {
                if (entry.second.IsFlag()) {
                    actualized_params.insert({entry.first, false});
                } else if (entry.second.IsRequired()) {
                    errors.emplace_back(std::string() + "Missing required field: [" +
                            Join(entry.second.short_forms()) + "] [" +
                            Join(entry.second.long_forms()) + "]");
                    continue;
                } else {
                    actualized_params.insert({entry.first, *entry.second.default_value()});
                }
            }
        }

        // Make sure all named arguments were passed in
        while (cur_named_param != named_params_.end()) {
            errors.emplace_back(std::string() + "Missing argument: " +
                    cur_named_param->second.name());
            cur_named_param++;
        }

        return ParseResult<T>(actualized_params, errors);
    }

    // Returns the validity of the Parser. If false, the parser was initialized incorrectly and
    // should not be used in production code. This value should most likely be deterministic at
    // runtime.
    operator bool() const {
        return valid_;
    }

  private:
    static const char kFormalArgumentSizeInvalid = -1;
    static const char kFormalArgumentSizeShort = 0;
    static const char kFormalArgumentSizeLong = 1;

    const std::map<T, FormalParameter> params_;
    // The following are simply caches of data for quicker access during parsing
    const std::map<std::string, T> short_forms_;
    const std::map<std::string, T> long_forms_;
    const std::map<std::string, T> short_flag_forms_;
    const std::map<std::string, T> long_flag_forms_;

    const std::map<T, NamedFormalParameter> named_params_;
    const bool valid_;


    explicit Parser<T>(const std::map<T, FormalParameter>& params,
                       const std::map<std::string, T>& short_forms,
                       const std::map<std::string, T>& long_forms,
                       const std::map<std::string, T>& short_flag_forms,
                       const std::map<std::string, T>& long_flag_forms,
                       const std::map<T, NamedFormalParameter>& named_params,
                       const bool valid) :
            params_(params),
            short_forms_(short_forms),
            long_forms_(long_forms),
            short_flag_forms_(short_flag_forms),
            long_flag_forms_(long_flag_forms),
            named_params_(named_params),
            valid_(valid) {}

    // Finds the first param that matches |form| and |arg_size|
    // Returns true if found and false if not. If true is returned, then ret will be updated with
    // the key for the param.
    bool FindFormalParameter(const std::string& form, const char arg_size, T& ret) const {
        for (const auto& entry : params_) {
            const T& key = entry.first;
            const FormalParameter& param = entry.second;
            if ((arg_size == kFormalArgumentSizeShort && param.HasShortForm(form)) ||
                        (arg_size == kFormalArgumentSizeLong && param.HasLongForm(form))) {
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
