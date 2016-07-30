// Copyright (c) 2016 Dustin Doloff
// Licensed under Apache License v2.0

#include "ahoy/type.h"

namespace ahoy {
namespace internal {

std::ostream& operator<<(std::ostream& os, const Type& type) {
    switch (type) {
        case Type::INVALID:
            return os << "Invalid";
        case Type::BOOL:
            return os << "Boolean";
        case Type::CHAR:
            return os << "Character";
        case Type::U_CHAR:
            return os << "Unsigned Character";
        case Type::SHORT:
            return os << "Short";
        case Type::U_SHORT:
            return os << "Unsigned Short";
        case Type::INT:
            return os << "Integer";
        case Type::U_INT:
            return os << "Unsigned Integer";
        case Type::LONG:
            return os << "Long";
        case Type::U_LONG:
            return os << "Unsigned Long";
        case Type::LONG_LONG:
            return os << "Long Long";
        case Type::U_LONG_LONG:
            return os << "Unsigned Long Long";
        case Type::FLOAT:
            return os << "Float";
        case Type::DOUBLE:
            return os << "Double";
        case Type::LONG_DOUBLE:
            return os << "Long Double";
        case Type::STRING:
            return os << "String";
        default:
            return os << "Unknown";
    }
}

} // namespace internal
} // namespace ahoy
