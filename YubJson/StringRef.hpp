//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 16.07.2016.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "YubJsonDefinitions.hpp"

#include <cstddef>
#include <iosfwd>

namespace YubJson
{
    struct StringRef
    {
        StringRef()
                : data(), length()
        {}

        StringRef(const char* data, size_t length)
                : data(data), length(length)
        {}

        const char* data;
        size_t length;
    };

    UBJSON_API std::ostream& operator<<(std::ostream& os, const StringRef& sr);

    UBJSON_API std::string getString(const StringRef& sr);
}
