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
        UndefinedValue = 0,
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
        ObjectValue = '{',
        ArrayValue = '['
    };

    inline std::ostream& operator<<(std::ostream& os, ValueType valueType)
    {
        return os << static_cast<char>(valueType);
    }
}
