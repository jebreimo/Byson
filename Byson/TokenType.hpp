//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2016-07-16 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <ostream>

namespace Byson
{
    enum class TokenType : char
    {
        UNKNOWN_TOKEN = 0,
        NULL_VALUE = 'Z',
        NOOP_VALUE = 'N',
        TRUE_VALUE = 'T',
        FALSE_VALUE = 'F',
        INT8_VALUE = 'i',
        UINT8_VALUE = 'U',
        INT16_VALUE = 'I',
        INT32_VALUE = 'l',
        INT64_VALUE = 'L',
        FLOAT32_VALUE = 'd',
        FLOAT64_VALUE = 'D',
        CHAR_VALUE = 'C',
        STRING_VALUE = 'S',
        BEGIN_OBJECT = '{',
        END_OBJECT = '}',
        BEGIN_ARRAY = '[',
        END_ARRAY = ']',
        COUNT = '#',
        VALUE_TYPE = '$'
    };

    inline std::ostream& operator<<(std::ostream& os, TokenType tokenType)
    {
        return os << static_cast<char>(tokenType);
    }
}
