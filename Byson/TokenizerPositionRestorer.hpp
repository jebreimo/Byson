//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2015-02-10 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "Tokenizer.hpp"

namespace Byson
{
    class TokenizerPositionRestorer
    {
    public:
        TokenizerPositionRestorer(Tokenizer& tokenizer) : m_Tokenizer(
                tokenizer), m_Position(tokenizer.position())
        {
        }

        ~TokenizerPositionRestorer()
        {
            restore();
        }

        void update()
        {
            m_Position = m_Tokenizer.position();
        }

        void clear()
        {
            m_Position = nullptr;
        }

        void restore()
        {
            if (m_Position)
                m_Tokenizer.setPosition(m_Position);
        }

    private:
        Tokenizer& m_Tokenizer;
        const void* m_Position;
    };
}
