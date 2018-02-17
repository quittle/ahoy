// Copyright (c) 2016, 2018 Dustin Doloff
// Licensed under Apache License v2.0

#ifndef AHOY_AHOY_INTERNAL_ARG_SIZE_H
#define AHOY_AHOY_INTERNAL_ARG_SIZE_H

namespace ahoy {
namespace internal {

// A representation of the size of an argument
enum class ArgSize {
    INVALID,
    // Example: "-v"
    SHORT,
    // Example: "--verbose"
    LONG,
};

} // namespace internal
} // namespace ahoy

#endif // AHOY_AHOY_INTERNAL_ARG_SIZE_H
