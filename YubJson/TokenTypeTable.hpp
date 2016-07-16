//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2014-04-24 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cstdint>
#include <vector>
#include "TokenType.hpp"
#include "YubJsonDefinitions.hpp"

namespace YubJson
{
    class TokenTypeTable
    {
    public:
        TokenTypeTable()
            : m_TokenTypes()
        {
            m_TokenTypes['#'] = static_cast<TokenType>('#');
            m_TokenTypes['$'] = static_cast<TokenType>('$');
            m_TokenTypes['['] = static_cast<TokenType>('[');
            m_TokenTypes[']'] = static_cast<TokenType>(']');
            m_TokenTypes['C'] = static_cast<TokenType>('C');
            m_TokenTypes['d'] = static_cast<TokenType>('d');
            m_TokenTypes['D'] = static_cast<TokenType>('D');
            m_TokenTypes['F'] = static_cast<TokenType>('F');
            m_TokenTypes['i'] = static_cast<TokenType>('i');
            m_TokenTypes['I'] = static_cast<TokenType>('I');
            m_TokenTypes['l'] = static_cast<TokenType>('l');
            m_TokenTypes['L'] = static_cast<TokenType>('L');
            m_TokenTypes['N'] = static_cast<TokenType>('N');
            m_TokenTypes['S'] = static_cast<TokenType>('S');
            m_TokenTypes['T'] = static_cast<TokenType>('T');
            m_TokenTypes['U'] = static_cast<TokenType>('U');
            m_TokenTypes['Z'] = static_cast<TokenType>('Z');
            m_TokenTypes['{'] = static_cast<TokenType>('{');
            m_TokenTypes['}'] = static_cast<TokenType>('}');
        }

        TokenType operator[](char marker) const
        {
            if (marker < 0)
                return TokenType::UnknownToken;
            return m_TokenTypes[marker];
        }

    private:
        TokenType m_TokenTypes[128];
    };
}
