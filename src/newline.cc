// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/newline.h"

namespace ahoy {
namespace internal {

std::string NewlineToString(const Newline newline) {
    Newline real_newline = newline;
    if (newline == Newline::AUTO) {
#if defined(WIN_32)
        real_newline = Newline::WINDOWS;
#else
        real_newline = Newline::POSIX;
#endif
    }

    if (real_newline == Newline::POSIX) {
        return "\n";
    } else if (real_newline == Newline::WINDOWS) {
        return "\r\n";
    } else {
        // Should never happen, but just in case
        return "\n";
    }
}

} // namespace internal
} // namespace ahoy

std::ostream& operator<<(std::ostream& os, const ahoy::Newline& newline) {
    return os << ahoy::internal::NewlineToString(newline);
}
