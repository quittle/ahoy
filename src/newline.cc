#include "ahoy/newline.h"

namespace ahoy {

std::string NewlineToString(Newline newline) {
    if (newline == Newline::AUTO) {
#if defined(WIN_32)
        newline = Newline::WINDOWS;
#else
        newline = Newline::POSIX;
#endif
    }

    if (newline == Newline::POSIX) {
        return "\n";
    } else if (newline == Newline::WINDOWS) {
        return "\r\n";
    } else {
        // Should never happen, but just in case
        return "\n";
    }
}

} // namespace ahoy
