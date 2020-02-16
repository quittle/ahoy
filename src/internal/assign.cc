// Copyright (c) 2018 Dustin Toff
// Licensed under Apache License v2.0

#include "ahoy/internal/assign.h"

#include <algorithm>
#include <cctype>
#include <climits>
#include <set>
#include <stdexcept>

namespace {

// Converts returns a lowercase version of the string passed in
std::string ToLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), ::tolower);
    return value;
}

// Checks if a string looks like it could be a negative number
bool is_negative(const std::string& value) {
    return value.size() > 0 && value[0] == '-';
}

template<typename T>
bool assign(T func(const std::string&, std::size_t*, int), T* const pointer,
        const std::string& value) {
    std::size_t leftover;
    // std::sto*** methods can parse strings partially, but this requires complete parsing
    *pointer = func(value, &leftover, 10);
    return leftover == value.size();
}

template<typename T>
bool assign(T func(const std::string&, std::size_t*), T* const pointer, const std::string& value) {
    std::size_t leftover;
    // std::sto*** methods can parse strings partially, but this requires complete parsing
    *pointer = func(value, &leftover);
    return leftover == value.size();
}

const std::set<std::string> kTrues = { "true", "t", "yes", "y", "on", "1" };
const std::set<std::string> kFalses = { "false", "f", "no", "n", "off", "0" };

} // namepspace

namespace ahoy {
namespace internal {

bool AssignBool(bool * const pointer, const std::string& value) {
    const std::string lower_value = ToLower(value);
    if (kTrues.find(lower_value) != kTrues.end()) {
        *pointer = true;
    } else if (kFalses.find(lower_value) != kFalses.end()) {
        *pointer = false;
    } else {
        return false;
    }
    return true;
}

bool AssignBool(bool * const pointer, const bool value) {
    *pointer = value;
    return true;
}

bool AssignChar(char * const pointer, const std::string& value) {
    if (value.size() != 1) {
        return false;
    }

    *pointer = value[0];
    return true;
}

bool AssignChar(char * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignUChar(unsigned char * const pointer, const std::string& value) {
    if (value.size() != 1) {
        return false;
    }

    *pointer = value[0];
    return true;
}

bool AssignUChar(unsigned char * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignShort(short * const pointer, const std::string& value) {
    try {
        int int_value;
        if (assign(&std::stoi, &int_value, value) &&
                int_value <= SHRT_MAX && int_value >= SHRT_MIN) {
            *pointer = int_value;
            return true;
        }
        return false;
    } catch (std::invalid_argument&) {
        return false;
    } catch (std::out_of_range&) {
        return false;
    }
}

bool AssignShort(short * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignUShort(unsigned short * const pointer, const std::string& value) {
    try {
        unsigned long ulong_value;
        if (assign(&std::stoul, &ulong_value, value) && ulong_value <= USHRT_MAX) {
            *pointer = ulong_value;
            return true;
        }
        return false;
    } catch (std::invalid_argument&) {
        return false;
    } catch (std::out_of_range&) {
        return false;
    }
}

bool AssignUShort(unsigned short * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignInt(int * const pointer, const std::string& value) {
    try {
        return assign(&std::stoi, pointer, value);
    } catch (std::invalid_argument&) {
        return false;
    } catch (std::out_of_range&) {
        return false;
    }
}

bool AssignInt(int * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignUInt(uint * const pointer, const std::string& value) {
    try {
        unsigned long ulong_value;
        if (assign(&std::stoul, &ulong_value, value) && ulong_value <= UINT_MAX) {
            *pointer = ulong_value;
            return true;
        }
        return false;
    } catch (std::invalid_argument&) {
        return false;
    } catch (std::out_of_range&) {
        return false;
    }
}

bool AssignUInt(uint * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignLong(long * const pointer, const std::string& value) {
    try {
        return assign(&std::stol, pointer, value);
    } catch (std::invalid_argument&) {
        return false;
    } catch (std::out_of_range&) {
        return false;
    }
}

bool AssignLong(long * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignULong(unsigned long * const pointer, const std::string& value) {
    if (is_negative(value)) {
        return false;
    }

    try {
        return assign(&std::stoul, pointer, value);
    } catch (std::invalid_argument&) {
        return false;
    } catch (std::out_of_range&) {
        return false;
    }
}

bool AssignULong(unsigned long * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignLongLong(long long * const pointer, const std::string& value) {
    try {
        return assign(&std::stoll, pointer, value);
    } catch (std::invalid_argument&) {
        return false;
    } catch (std::out_of_range&) {
        return false;
    }
}

bool AssignLongLong(long long * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignULongLong(unsigned long long * const pointer, const std::string& value) {
    if (is_negative(value)) {
        return false;
    }

    try {
        return assign(&std::stoull, pointer, value) && *pointer <= ULLONG_MAX;
    } catch (std::invalid_argument&) {
        return false;
    } catch (std::out_of_range&) {
        return false;
    }
}

bool AssignULongLong(unsigned long long * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignFloat(float * const pointer, const std::string& value) {
    try {
        // return assign(&std::stof, pointer, value);
        std::size_t leftover(0);
        *pointer = std::stof(value, &leftover);
        if (leftover != value.size()) {
            return false;
        }
        return true;
    } catch (std::invalid_argument&) {
        return false;
    } catch (std::out_of_range&) {
        return false;
    }
}

bool AssignFloat(float * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignDouble(double * const pointer, const std::string& value) {
    try {
        return assign(&std::stod, pointer, value);
    } catch (std::invalid_argument&) {
        return false;
    } catch (std::out_of_range&) {
        return false;
    }
}

bool AssignDouble(double * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignLongDouble(long double * const pointer, const std::string& value) {
    try {
        return assign(&std::stold, pointer, value);
    } catch (std::invalid_argument&) {
        return false;
    } catch (std::out_of_range&) {
        return false;
    }
}

bool AssignLongDouble(long double * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignString(std::string * const pointer, const std::string& value) {
    *pointer = value;
    return true;
}

bool AssignString(std::string * const pointer, const bool value) {
    *pointer = value ? "true" : "false";
    return true;
}

} // namespace internal
} // namespace ahoy
