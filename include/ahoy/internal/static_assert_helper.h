// Copyright (c) 2016, 2018 Dustin Toff
// Licensed under apache License v2.0

#ifndef AHOY_AHOY_INTERNAL_STATIC_ASSERT_HELPER_H
#define AHOY_AHOY_INTERNAL_STATIC_ASSERT_HELPER_H

#include <type_traits>

// This class contains structs to parse templated varargs for values
// Example usage: static_assert(does_contain_type_2<int, bool, args...>::value, "Reason");

namespace ahoy {
namespace internal {

// Forward declare the checkers
template<typename... t> struct does_contain_type_1;
template<typename... t> struct does_contain_type_2;

// Occurs when |other| is not the same as |a|.
// Continue looking for |a| in |tail|.
template<typename a, typename other, typename... tail>
struct does_contain_type_1<a, other, tail...> : does_contain_type_1<a, tail...> {};

// Occurs when |a| is found.
// Return true.
template<typename a, typename... tail>
struct does_contain_type_1<a, a, tail...> : std::true_type {};

// Occurs when the end of the varargs has been reached and |a| was not found.
// Return false.
template<typename a> struct does_contain_type_1<a> : std::false_type {};

// Occurs when |a| is found, but |b| has not.
// Begin looking exclusively for |b|.
template<typename a, typename b, typename... tail>
struct does_contain_type_2<a, b, a, tail...> : does_contain_type_1<b, tail...> {};

// Occurs when |b| is found, but |a| has not.
// Begin looking exclusively for |a|.
template<typename a, typename b, typename... tail>
struct does_contain_type_2<a, b, b, tail...> : does_contain_type_1<a, tail...> {};

// Occurs when neither |a| nor |b| were found.
// Continue looking for either.
template<typename a, typename b, typename other, typename... tail>
struct does_contain_type_2<a, b, other, tail...> : does_contain_type_2<a, b, tail...> {};

// Occurs when the end of the varargs has been reached but neither |a| nor |b| were found.
// Return false.
template<typename a, typename b> struct does_contain_type_2<a, b> : std::false_type {};

} // namespace internal
} // namespace ahoy

#endif // AHOY_AHOY_INTERNAL_STATIC_ASSERT_HELPER_H
