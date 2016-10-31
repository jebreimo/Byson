//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 16.07.2016.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <ostream>

namespace Byson
{
    enum class ValueType : char
    {
        UNDEFINED_VALUE = 0,
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
        OBJECT_VALUE = '{',
        ARRAY_VALUE = '['
    };

    inline std::ostream& operator<<(std::ostream& os, ValueType valueType)
    {
        return os << static_cast<char>(valueType);
    }
}
