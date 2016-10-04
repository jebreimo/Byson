//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 16.07.2016.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "StringRef.hpp"

#include <ostream>

namespace Byson
{
    std::string getString(const StringRef& sr)
    {
        return std::string(sr.data, sr.length);
    }

    std::ostream& operator<<(std::ostream& os, const StringRef& sr)
    {
        if (sr.length)
            os.write(sr.data, sr.length);
        return os;
    }
}
