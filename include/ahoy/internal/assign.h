// Copyright (c) 2018 Dustin Toff
// Licensed under Apache License v2.0

#ifndef AHOY_AHOY_INTERNAL_ASSIGN_H
#define AHOY_AHOY_INTERNAL_ASSIGN_H

#include <string>

#include "ahoy/internal/type.h"

namespace ahoy {
namespace internal {

// The following are implementations of how to convert strings and booleans to various types.
// For booleans, strings checked if they match one of several affirmation strings.
// For chars, booleans are converted to 1 and 0 and strings are expected to be a single character
// long.
// For numeric types, booleans are converted to 1 and 0 and strings are parsed to numbers
//
// Consumers should prefer using the |Assign| method at the bottom of this file instead as a general
// assignment method.

bool AssignBool(bool * const pointer, const std::string& value);
bool AssignBool(bool * const pointer, const bool value);

bool AssignChar(char * const pointer, const std::string& value);
bool AssignChar(char * const pointer, const bool value);

bool AssignUChar(unsigned char * const pointer, const std::string& value);
bool AssignUChar(unsigned char * const pointer, const bool value);

bool AssignShort(short * const pointer, const std::string& value);
bool AssignShort(short * const pointer, const bool value);

bool AssignUShort(unsigned short * const pointer, const std::string& value);
bool AssignUShort(unsigned short * const pointer, const bool value);

bool AssignInt(int * const pointer, const std::string& value);
bool AssignInt(int * const pointer, const bool value);

bool AssignUInt(uint * const pointer, const std::string& value);
bool AssignUInt(uint * const pointer, const bool value);

bool AssignLong(long * const pointer, const std::string& value);
bool AssignLong(long * const pointer, const bool value);

bool AssignULong(unsigned long * const pointer, const std::string& value);
bool AssignULong(unsigned long * const pointer, const bool value);

bool AssignLongLong(long long * const pointer, const std::string& value);
bool AssignLongLong(long long * const pointer, const bool value);

bool AssignULongLong(unsigned long long * const pointer, const std::string& value);
bool AssignULongLong(unsigned long long * const pointer, const bool value);

bool AssignFloat(float * const pointer, const std::string& value);
bool AssignFloat(float * const pointer, const bool value);

bool AssignDouble(double * const pointer, const std::string& value);
bool AssignDouble(double * const pointer, const bool value);

bool AssignLongDouble(long double * const pointer, const std::string& value);
bool AssignLongDouble(long double * const pointer, const bool value);

bool AssignString(std::string * const pointer, const std::string& value);
bool AssignString(std::string * const pointer, const bool value);

// Maps a Type to an assignment method and returns the success of the assignment
template<typename T>
bool Assign(void * const pointer, const ahoy::internal::Type type, const T& value) {
    switch (type) {
        case ahoy::internal::Type::BOOL:
            return AssignBool((bool*) pointer, value);
        case ahoy::internal::Type::CHAR:
            return AssignChar((char*) pointer, value);
        case ahoy::internal::Type::U_CHAR:
            return AssignUChar((unsigned char*) pointer, value);
        case ahoy::internal::Type::SHORT:
            return AssignShort((short*) pointer, value);
        case ahoy::internal::Type::U_SHORT:
            return AssignUShort((unsigned short*) pointer, value);
        case ahoy::internal::Type::INT:
            return AssignInt((int*) pointer, value);
        case ahoy::internal::Type::U_INT:
            return AssignUInt((unsigned int*) pointer, value);
        case ahoy::internal::Type::LONG:
            return AssignLong((long*) pointer, value);
        case ahoy::internal::Type::U_LONG:
            return AssignULong((unsigned long*) pointer, value);
        case ahoy::internal::Type::LONG_LONG:
            return AssignLongLong((long long*) pointer, value);
        case ahoy::internal::Type::U_LONG_LONG:
            return AssignULongLong((unsigned long long*) pointer, value);
        case ahoy::internal::Type::FLOAT:
            return AssignFloat((float*) pointer, value);
        case ahoy::internal::Type::DOUBLE:
            return AssignDouble((double*) pointer, value);
        case ahoy::internal::Type::LONG_DOUBLE:
            return AssignLongDouble((long double*) pointer, value);
        case ahoy::internal::Type::STRING:
            return AssignString((std::string*) pointer, value);
        default:
            return false;
    }
}

} // namespace internal
} // namespace ahoy

#endif // AHOY_AHOY_INTERNAL_ASSIGN_H
