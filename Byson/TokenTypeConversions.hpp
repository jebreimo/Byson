//****************************************************************************
// Copyright © 2016 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 16.07.2016.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "TokenType.hpp"
#include "ValueType.hpp"

#include <cstdint>

namespace YubJson
{
    template <typename T>
    struct TypeToValueType
    {
    };

    template <typename T>
    struct TypeToTokenType
    {
    };

    template <ValueType valueType>
    struct ValueTypeToType
    {
    };

    template <TokenType tokenType>
    struct TokenTypeToType
    {
    };

    #define YUBJ_DEFINE_CORRESPONDING_CPP_TYPES(cppType, ubjsonType) \
        template <> \
        struct ValueTypeToType<ValueType::ubjsonType> \
        { \
            typedef cppType Type; \
        }; \
        template <> \
        struct TokenTypeToType<TokenType::ubjsonType> \
        { \
            typedef cppType Type; \
        }

    #define YUBJ_DEFINE_CORRESPONDING_UBJSON_TYPES(cppType, ubjsonType) \
        template <> \
        struct TypeToValueType<cppType> \
        { \
            static const ValueType valueType = ValueType::ubjsonType; \
        }; \
        template <> \
        struct TypeToTokenType<cppType> \
        { \
            static const TokenType tokenType = TokenType::ubjsonType; \
        }

    #define YUBJ_DEFINE_CORRESPONDING_TYPES(cppType, ubjsonType) \
        YUBJ_DEFINE_CORRESPONDING_CPP_TYPES(cppType, ubjsonType); \
        YUBJ_DEFINE_CORRESPONDING_UBJSON_TYPES(cppType, ubjsonType)

    YUBJ_DEFINE_CORRESPONDING_TYPES(int8_t, Int8Value);
    YUBJ_DEFINE_CORRESPONDING_TYPES(uint8_t, UInt8Value);
    YUBJ_DEFINE_CORRESPONDING_TYPES(int16_t, Int16Value);
    YUBJ_DEFINE_CORRESPONDING_TYPES(int32_t, Int32Value);
    YUBJ_DEFINE_CORRESPONDING_TYPES(int64_t, Int64Value);
    YUBJ_DEFINE_CORRESPONDING_TYPES(float, Float32Value);
    YUBJ_DEFINE_CORRESPONDING_TYPES(double, Float64Value);
    YUBJ_DEFINE_CORRESPONDING_TYPES(char, CharValue);
    YUBJ_DEFINE_CORRESPONDING_CPP_TYPES(bool, FalseValue);
    YUBJ_DEFINE_CORRESPONDING_CPP_TYPES(bool, TrueValue);
    YUBJ_DEFINE_CORRESPONDING_UBJSON_TYPES(long, Int32Value);
    YUBJ_DEFINE_CORRESPONDING_UBJSON_TYPES(unsigned, Int32Value);
}
