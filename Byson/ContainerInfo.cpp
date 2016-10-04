//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 16.07.2016.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "ContainerInfo.hpp"

#include <ostream>

namespace YubJson
{
    bool operator==(const ContainerInfo& a, const ContainerInfo& b)
    {
        return a.length == b.length && a.type == b.type;
    }

    bool operator!=(const ContainerInfo& a, const ContainerInfo& b)
    {
        return !(a == b);
    }

    std::ostream& operator<<(std::ostream& os, const ContainerInfo& info)
    {
        return os << "{\"length\": " << info.length << ", \"type\": \""
                  << (char)info.type << "\"}";
    }
}
