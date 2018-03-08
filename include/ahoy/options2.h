// Copyright (c) 2016, 2018 Dustin Doloff
// Licensed under Apache License v2.0

#ifndef AHOY_AHOY_OPTIONS2_H
#define AHOY_AHOY_OPTIONS2_H

#include <list>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "ahoy/options.h"
#include "ahoy/internal/assign.h"
#include "ahoy/internal/formal_parameter.h"
#include "ahoy/internal/static_assert_helper.h"

#include <iostream> // REMOVE ME

#define _AHOY_OPTIONS2_STATIC_ASSERT_NOT_BOTH(a, b) \
    static_assert(!ahoy::internal::does_contain_type_2<b, a, Options...>::value, \
                  "Named parameter may not be both an #a and #b.")

// Validates the varargs of options
template <typename... Options>
class OptionChecker {
    _AHOY_OPTIONS2_STATIC_ASSERT_NOT_BOTH(ahoy::Flag, ahoy::Required);
    _AHOY_OPTIONS2_STATIC_ASSERT_NOT_BOTH(ahoy::ShortForms, ahoy::Flag);
    _AHOY_OPTIONS2_STATIC_ASSERT_NOT_BOTH(ahoy::LongForms, ahoy::Flag);
    // // Check that Options does not contain both ahoy::Flag and ahoy::Required
    // static_assert(!ahoy::internal::does_contain_type_2<ahoy::Flag, ahoy::Required, Options...>::value,
    //               "Named parameter may not be both an ahoy::Flag and ahoy::Required.");
    // static_assert(!ahoy::internal::does_contain_type_2<ahoy::ShortForms, ahoy::Marker, Options...>::value,
    //               "Named parameter may not be both an ahoy::Flag and ahoy::Required.");
    // static_assert(!ahoy::internal::does_contain_type_2<ahoy::LongForms, ahoy::Marker, Options...>::value,
    //               "Named parameter may not be both an ahoy::Flag and ahoy::Required.");
};

#define _AHOY_PARSER_BUILD_FORMAL_PARAMETER(OptionClass, formal_parameter_value) \
    template<class ...Options> \
    static void BuildFormalNamedParameter(internal::FormalParameter* fp, const OptionClass& option_value, \
                                          Options... options) { \
        fp->formal_parameter_value(option_value.get()); \
        BuildFormalNamedParameter(fp, options...); \
    }

#define _AHOY_OPTION2_CTR(PointerType, InternalType) \
    template<class ...Options> \
    explicit Option2(PointerType* storage, Options... options) { \
        OptionChecker<Options...>{}; \
        fp_.type(InternalType); \
        BuildFormalNamedParameter(&fp_, options...); \
        storage_ = storage; \
    }

namespace ahoy {

class Option2 {
   public:
    _AHOY_OPTION2_CTR(bool, internal::Type::BOOL);
    _AHOY_OPTION2_CTR(char, internal::Type::CHAR);
    _AHOY_OPTION2_CTR(unsigned char, internal::Type::U_CHAR);
    _AHOY_OPTION2_CTR(short, internal::Type::SHORT);
    _AHOY_OPTION2_CTR(unsigned short, internal::Type::U_SHORT);
    _AHOY_OPTION2_CTR(int, internal::Type::INT);
    _AHOY_OPTION2_CTR(unsigned int, internal::Type::U_INT);
    _AHOY_OPTION2_CTR(long, internal::Type::LONG);
    _AHOY_OPTION2_CTR(unsigned long, internal::Type::U_LONG);
    _AHOY_OPTION2_CTR(long long, internal::Type::LONG_LONG);
    _AHOY_OPTION2_CTR(unsigned long long, internal::Type::U_LONG_LONG);
    _AHOY_OPTION2_CTR(float, internal::Type::FLOAT);
    _AHOY_OPTION2_CTR(double, internal::Type::DOUBLE);
    _AHOY_OPTION2_CTR(long double, internal::Type::LONG_DOUBLE);
    _AHOY_OPTION2_CTR(std::string, internal::Type::STRING);

    virtual ~Option2() {}

    // TODO: Convert to vararg
    Option2& withOptions(const std::vector<Option2>& options) {
        current_options_ = options;
        return *this;
    }

    template <typename... O>
    Option2& withOptions(O&&... options) {
        return withOptions({ std::forward<Option2>(options)... });
    }

    Option2& then(const std::vector<Option2>& options) {
        next_options_ = options;
        return *this;
    }

    template <typename... O>
    Option2& then(O&&... options) {
        return then({ std::forward<Option2>(options)... });
    }

    bool consume(std::list<std::string>& args) const {
        bool failed = false;
        bool consumed = false;

        if (must_consume_) {
            consumed = true;
        } else {
            if (fp_.is_positional() && args.size() >= 1) {
                if (fp_.flag()) {
                    failed = !Assign(storage_, fp_.type(), true);
                } else {
                    failed = !Assign(storage_, fp_.type(), args.front());
                }
                args.pop_front();
                consumed = true;
            } else if (args.size() >= 2) {
                const std::string arg = args.front();
                if (fp_.forms().count(arg)) {
                    args.pop_front();
                    if (fp_.flag()) {
                        failed = !Assign(storage_, fp_.type(), true);
                        consumed = true;
                    } else {
                        if (args.size() == 0) {
                          failed = true;
                        } else {
                            failed = !Assign(storage_, fp_.type(), args.front());
                            args.pop_front();
                            consumed = true;
                        }
                    }
                }
            }
        }

        if (consumed) {
            for (const Option2& option : current_options_) {
                option.consume(args);
            }

            if (next_options_.size() == 0) {
                return true;
            }

            for (const Option2& option : next_options_) {
                if (option.consume(args)) {
                    return true;
                }
            }
        }

        return false;
    }

    bool must_consume_ = false;

  private:
    // _AHOY_PARSER_BUILD_FORMAL_PARAMETER(ahoy::Marker, marker)
    _AHOY_PARSER_BUILD_FORMAL_PARAMETER(ahoy::LongForms, long_forms)
    _AHOY_PARSER_BUILD_FORMAL_PARAMETER(ahoy::ShortForms, short_forms)
    _AHOY_PARSER_BUILD_FORMAL_PARAMETER(ahoy::Name, name)
    _AHOY_PARSER_BUILD_FORMAL_PARAMETER(ahoy::Description, description)
    _AHOY_PARSER_BUILD_FORMAL_PARAMETER(ahoy::Required, required)
    _AHOY_PARSER_BUILD_FORMAL_PARAMETER(ahoy::Flag, flag)
    static void BuildFormalNamedParameter(internal::FormalParameter*) {}

    void* storage_;
    ::ahoy::internal::FormalParameter fp_;

    std::vector<Option2> current_options_;
    std::vector<Option2> next_options_;
};

} // namespace ahoy

#endif // AHOY_AHOY_OPTIONS2_H
