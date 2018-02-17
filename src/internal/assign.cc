// Copyright (c) 2018 Dustin Doloff
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

} // namepspace

namespace ahoy {
namespace internal {

bool AssignBool(bool * const pointer, const std::string& value) {
    const std::set<std::string> trues = { "true", "t", "yes", "y", "on", "1" };
    *pointer = trues.find(ToLower(value)) != trues.end();
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
        const int int_value = std::stoi(value);
        if (int_value > SHRT_MAX || int_value < SHRT_MIN) {
            return false;
        }
        *pointer = int_value;
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignShort(short * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignUShort(unsigned short * const pointer, const std::string& value) {
    try {
        const unsigned long ulong_value = std::stoul(value);
        if (ulong_value > USHRT_MAX) {
            return false;
        }
        *pointer = ulong_value;
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignUShort(unsigned short * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignInt(int * const pointer, const std::string& value) {
    try {
        *pointer = std::stoi(value);
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignInt(int * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignUInt(uint * const pointer, const std::string& value) {
    try {
        const unsigned long ulong_value = std::stoul(value);
        if (ulong_value > UINT_MAX) {
            return false;
        }
        *pointer = ulong_value;
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignUInt(uint * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignLong(long * const pointer, const std::string& value) {
    try {
        *pointer = std::stol(value);
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignLong(long * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignULong(unsigned long * const pointer, const std::string& value) {
    try {
        *pointer = std::stoul(value);
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignULong(unsigned long * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignLongLong(long long * const pointer, const std::string& value) {
    try {
        *pointer = std::stoll(value);
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignLongLong(long long * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignULongLong(unsigned long long * const pointer, const std::string& value) {
    try {
        *pointer = std::stoull(value);
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignULongLong(unsigned long long * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignFloat(float * const pointer, const std::string& value) {
    try {
        *pointer = std::stof(value);
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignFloat(float * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignDouble(double * const pointer, const std::string& value) {
    try {
        *pointer = std::stod(value);
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
        return false;
    }
}

bool AssignDouble(double * const pointer, const bool value) {
    *pointer = value ? 1 : 0;
    return true;
}

bool AssignLongDouble(long double * const pointer, const std::string& value) {
    try {
        *pointer = std::stold(value);
        return true;
    } catch (std::invalid_argument) {
        return false;
    } catch (std::out_of_range) {
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
