// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#ifndef AHOY_AHOY_NEWLINE_H
#define AHOY_AHOY_NEWLINE_H

#include <string>
#include <ostream>

namespace ahoy {

// Identifier for representation of a newline
enum class Newline {
    // Auto-detect the correct platform
    AUTO,
    // Posix newline is "\n"
    POSIX,
    // Windows newline is "\r\n"
    WINDOWS,
};

namespace internal {

// Converts a Newline to its string representation
std::string NewlineToString(const Newline newline);

} // namespace internal
} // namespace ahoy

// Writes the appropriate newline to the ostream
std::ostream& operator<<(std::ostream& os, const ahoy::Newline& newline);

#endif // AHOY_AHOY_NEWLINE_H
