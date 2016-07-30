// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#ifndef AHOY_AHOY_TYPE_H
#define AHOY_AHOY_TYPE_H

#include <ostream>

namespace ahoy {
namespace internal {

// The type of a parameter
enum class Type {
    INVALID,
    BOOL,
    CHAR,
    U_CHAR,
    SHORT,
    U_SHORT,
    INT,
    U_INT,
    LONG,
    U_LONG,
    LONG_LONG,
    U_LONG_LONG,
    FLOAT,
    DOUBLE,
    LONG_DOUBLE,
    STRING,
};

// Writes a string form of |type| to the ostream
std::ostream& operator<<(std::ostream& os, const Type& type);

} // namespace internal
} // namespace ahoy

#endif // AHOY_AHOY_TYPE_H
