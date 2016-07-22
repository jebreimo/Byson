//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2016-07-16 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <ostream>

namespace YubJson
{
    enum class TokenType : char
    {
        UnknownToken = 0,
        NullValue = 'Z',
        NoOpValue = 'N',
        TrueValue = 'T',
        FalseValue = 'F',
        Int8Value = 'i',
        UInt8Value = 'U',
        Int16Value = 'I',
        Int32Value = 'l',
        Int64Value = 'L',
        Float32Value = 'd',
        Float64Value = 'D',
        CharValue = 'C',
        StringValue = 'S',
        BeginObject = '{',
        EndObject = '}',
        BeginArray = '[',
        EndArray = ']',
        Count = '#',
        ValueType = '$'
    };

    inline std::ostream& operator<<(std::ostream& os, TokenType tokenType)
    {
        return os << static_cast<char>(tokenType);
    }
}
