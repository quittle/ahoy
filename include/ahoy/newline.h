#ifndef AHOY_AHOY_NEWLINE_H
#define AHOY_AHOY_NEWLINE_H

#include <string>

namespace ahoy {

enum class Newline {
    AUTO,
    POSIX,
    WINDOWS
};

std::string NewlineToString(Newline newline);

} // namespace ahoy

#endif // AHOY_AHOY_NEWLINE_H
