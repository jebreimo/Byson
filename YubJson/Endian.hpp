//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 07.08.2016.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cstdint>
#include <algorithm>
#include <type_traits>

namespace YubJson
{
    #ifdef IS_BIG_ENDIAN

    static const bool IsBigEndian = true;

    #else

    static const bool IsBigEndian = false;

    #endif

    static const bool IsLittleEndian = !IsBigEndian;

    template <size_t N>
    inline void reverseBytes(char* value)
    {}

    template <>
    inline void reverseBytes<2>(char* value)
    {
        std::swap(value[0], value[1]);
    }

    template <>
    inline void reverseBytes<4>(char* value)
    {
        std::swap(value[0], value[3]);
        std::swap(value[1], value[2]);
    }

    template <>
    inline void reverseBytes<8>(char* value)
    {
        std::swap(value[0], value[7]);
        std::swap(value[1], value[6]);
        std::swap(value[2], value[5]);
        std::swap(value[3], value[4]);
    }

    template <typename T, bool DoSwap>
    struct EndiannessConverter;

    template <typename T>
    struct EndiannessConverter<T, true>
    {
        T operator()(T value) const
        {
            reverseBytes<sizeof(T)>(reinterpret_cast<char*>(&value));
            return value;
        }
    };

    template <typename T>
    struct EndiannessConverter<T, false>
    {
        constexpr T operator()(T value)
        {
            return value;
        }
    };

    template <typename T>
    T bigEndian(T value)
    {
        return EndiannessConverter<T, IsLittleEndian>()(value);
    }

    template <typename T>
    T littleEndian(T value)
    {
        return EndiannessConverter<T, IsBigEndian>()(value);
    }

    template <typename T, bool DoSwap>
    struct EndianCopier;

    template <typename T>
    struct EndianCopier<T, true>
    {
        void operator()(const T* value, char* destination) const
        {
            auto source = reinterpret_cast<const char*>(value);
            for (size_t i = 0; i < sizeof(T); ++i)
                destination[i] = source[sizeof(T) - 1 - i];
        }
    };

    template <typename T>
    struct EndianCopier<T, false>
    {
        void operator()(const T* value, char* destination) const
        {
            auto source = reinterpret_cast<const char*>(&value);
            for (size_t i = 0; i < sizeof(T); ++i)
                destination[i] = source[i];
        }
    };

    template <typename T>
    char* copyBigEndianValues(const T* values, size_t count,
                              char* destination)
    {
        EndianCopier<T, IsLittleEndian> copy;
        for (size_t i = 0; i < count; ++i)
        {
            copy(values + i, destination);
            destination += sizeof(T);
        }
        return destination;
    }

    template <typename T>
    char* copyLittleEndianValues(const T* values, size_t count,
                                 char* destination)
    {
        EndianCopier<T, IsBigEndian> copy;
        for (size_t i = 0; i < count; ++i)
        {
            copy(values + i, destination);
            destination += sizeof(T);
        }
        return destination;
    }
}
