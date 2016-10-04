//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 16.07.2016.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "ValueType.hpp"
#include "BysonDefinitions.hpp"

#include <cstdint>
#include <iosfwd>

namespace YubJson
{
    struct ContainerInfo
    {
        ContainerInfo()
            : length(),
              type(ValueType::UndefinedValue)
        {}

        ContainerInfo(int64_t length, ValueType type)
            : length(length),
              type(type)
        {}

        int64_t length;
        ValueType type;
    };

    BYSON_API bool operator==(const ContainerInfo& a, const ContainerInfo& b);

    BYSON_API bool operator!=(const ContainerInfo& a, const ContainerInfo& b);

    BYSON_API std::ostream& operator<<(std::ostream& os, const ContainerInfo& info);
}
