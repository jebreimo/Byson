//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2014-04-24 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "TokenType.hpp"
#include "BysonDefinitions.hpp"

#include <cstdint>
#include <utility>

namespace Byson
{
    class BYSON_API Tokenizer
    {
    public:
        Tokenizer(const void* buffer, size_t size);

        bool nextToken();

        TokenType tokenType() const;

        std::pair<bool, TokenType> peekToken() const;

        const void* position() const;

        void setPosition(const void* pos);

        bool isEndOfBuffer() const;

        bool advance(size_t bytes);

        template <typename T>
        bool read(T& value)
        {
          if (m_End - m_Pos < static_cast<ptrdiff_t>(sizeof(T)))
            return false;
          m_Pos += sizeof(T);
          auto alias = reinterpret_cast<uint8_t*>(&value);
          for (auto i = 0; i < sizeof(T); ++i)
            alias[i] = *(m_Pos - i - 1);
          return true;
        }

        template <typename T>
        bool read(T* buffer, size_t count)
        {
          if (m_End - m_Pos < static_cast<ptrdiff_t>(count * sizeof(T)))
            return false;
          auto alias = reinterpret_cast<uint8_t*>(buffer);
          auto index = sizeof(T);
          for (auto i = 0; i < count; ++i)
          {
            for (auto j = 0; j < sizeof(T); ++j)
              alias[--index] = *m_Pos++;
            index += 2 * sizeof(T);
          }
          return true;
        }

        bool read(uint8_t* buffer, size_t count);

        bool read(int8_t* buffer, size_t count);

        bool read(char* buffer, size_t count);

    private:
        bool readBytes(void* buffer, size_t size);

        const uint8_t* m_Pos;
        const uint8_t* m_End;
        TokenType m_TokenType;
    };
}
