//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2014-04-24 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Tokenizer.hpp"

#include "TokenTypeTable.hpp"

namespace YubJson
{
    namespace
    {
        static TokenTypeTable s_TokenTypeTable;
    }

    Tokenizer::Tokenizer(const void* buffer, size_t size)
        : m_Pos(static_cast<const uint8_t*>(buffer)),
          m_End(static_cast<const uint8_t*>(buffer) + size),
          m_TokenType(TokenType::UnknownToken)
    {}

    bool Tokenizer::nextToken()
    {
        if (m_Pos == m_End)
            return false;
        m_TokenType = s_TokenTypeTable[*m_Pos++];
        return true;
    }

    TokenType Tokenizer::getTokenType() const
    {
        return m_TokenType;
    }

    std::pair<bool, TokenType> Tokenizer::peekToken() const
    {
        if (m_Pos == m_End)
            return std::make_pair(false, TokenType::UnknownToken);
        return std::make_pair(true, s_TokenTypeTable[*m_Pos]);
    }

    const void* Tokenizer::getPosition() const
    {
        return m_Pos;
    }

    bool Tokenizer::isEndOfBuffer() const
    {
        return m_Pos == m_End;
    }

    void Tokenizer::setPosition(const void* pos)
    {
        m_Pos = static_cast<const uint8_t*>(pos);
        m_TokenType = TokenType::UnknownToken;
    }

    bool Tokenizer::advance(size_t bytes)
    {
        m_Pos += bytes;
        if (m_Pos > m_End)
        {
            m_Pos = m_End;
            return false;
        }
        return true;
    }

    bool Tokenizer::read(uint8_t* buffer, size_t count)
    {
        return readBytes(buffer, count);
    }

    bool Tokenizer::read(int8_t* buffer, size_t count)
    {
        return readBytes(buffer, count);
    }

    bool Tokenizer::read(char* buffer, size_t count)
    {
        return readBytes(buffer, count);
    }

    bool Tokenizer::readBytes(void* buffer, size_t size)
    {
        if (m_End - m_Pos < static_cast<ptrdiff_t>(size))
            return false;
        memcpy(buffer, m_Pos, size);
        m_Pos += size;
        return true;
    }
}
