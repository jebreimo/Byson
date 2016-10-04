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
#include "BysonDefinitions.hpp"

namespace Byson
{
    class TokenTypeTable
    {
    public:
        TokenTypeTable()
            : m_TokenTypes()
        {
            m_TokenTypes[int('#')] = static_cast<TokenType>('#');
            m_TokenTypes[int('$')] = static_cast<TokenType>('$');
            m_TokenTypes[int('[')] = static_cast<TokenType>('[');
            m_TokenTypes[int(']')] = static_cast<TokenType>(']');
            m_TokenTypes[int('C')] = static_cast<TokenType>('C');
            m_TokenTypes[int('d')] = static_cast<TokenType>('d');
            m_TokenTypes[int('D')] = static_cast<TokenType>('D');
            m_TokenTypes[int('F')] = static_cast<TokenType>('F');
            m_TokenTypes[int('i')] = static_cast<TokenType>('i');
            m_TokenTypes[int('I')] = static_cast<TokenType>('I');
            m_TokenTypes[int('l')] = static_cast<TokenType>('l');
            m_TokenTypes[int('L')] = static_cast<TokenType>('L');
            m_TokenTypes[int('N')] = static_cast<TokenType>('N');
            m_TokenTypes[int('S')] = static_cast<TokenType>('S');
            m_TokenTypes[int('T')] = static_cast<TokenType>('T');
            m_TokenTypes[int('U')] = static_cast<TokenType>('U');
            m_TokenTypes[int('Z')] = static_cast<TokenType>('Z');
            m_TokenTypes[int('{')] = static_cast<TokenType>('{');
            m_TokenTypes[int('}')] = static_cast<TokenType>('}');
        }

        TokenType operator[](char marker) const
        {
            if (marker < 0)
                return TokenType::UnknownToken;
            return m_TokenTypes[int(marker)];
        }

    private:
        TokenType m_TokenTypes[128];
    };
}
