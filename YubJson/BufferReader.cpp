//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2015-02-10 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "BufferReader.hpp"

#include <ostream>

namespace YubJson
{
    BufferReader::BufferReader(const void* buffer, size_t size)
        : m_Tokenizer(buffer, size), m_Buffer(buffer)
    {}

    BufferReader::~BufferReader()
    {}

    size_t BufferReader::position() const
    {
        auto diff = (const char*)m_Tokenizer.position()
                    - (const char*)m_Buffer;
        return (size_t)diff;
    }

    void BufferReader::setPosition(size_t pos)
    {
        m_Tokenizer.setPosition((const char*)m_Buffer + pos);
    }

    bool BufferReader::isEndOfBuffer() const
    {
        return m_Tokenizer.isEndOfBuffer();
    }

    bool BufferReader::readBeginObject(ContainerInfo& info)
    {
        TokenizerPositionRestorer tokRestorer(m_Tokenizer);
        if (!readTokenType(TokenType::BeginObject))
            return false;
        if (!readContainerInfo(info))
            return false;
        tokRestorer.clear();
        return true;
    }

    bool BufferReader::readEndObject()
    {
        TokenizerPositionRestorer tokRestorer(m_Tokenizer);
        if (!readTokenType(TokenType::EndObject))
            return false;
        tokRestorer.clear();
        return true;
    }

    bool BufferReader::isEndObject() const
    {
        return m_Tokenizer.peekToken()
               == std::make_pair(true, TokenType::EndObject);
    }

    bool BufferReader::readBeginArray(ContainerInfo& info)
    {
        TokenizerPositionRestorer tokRestorer(m_Tokenizer);
        if (!readTokenType(TokenType::BeginArray))
            return false;
        if (!readContainerInfo(info))
            return false;
        tokRestorer.clear();
        return true;
    }

    bool BufferReader::readEndArray()
    {
        TokenizerPositionRestorer tokRestorer(m_Tokenizer);
        if (!readTokenType(TokenType::EndArray))
            return false;
        tokRestorer.clear();
        return true;
    }

    bool BufferReader::isEndArray() const
    {
        return m_Tokenizer.peekToken() == std::make_pair(true, TokenType::EndArray);
    }

    bool BufferReader::readKey(StringRef& stringRef)
    {
        return readRawValue(stringRef);
    }

    bool BufferReader::readNull()
    {
        TokenizerPositionRestorer tokRestorer(m_Tokenizer);
        if (!readTokenType(TokenType::NullValue))
            return false;
        tokRestorer.clear();
        return true;
    }

    bool BufferReader::readNoOp()
    {
        TokenizerPositionRestorer tokRestorer(m_Tokenizer);
        if (!readTokenType(TokenType::NoOpValue))
            return false;
        tokRestorer.clear();
        return true;
    }

    bool BufferReader::readValue(bool& value)
    {
        TokenizerPositionRestorer tokRestorer(m_Tokenizer);
        if (!m_Tokenizer.nextToken())
            return false;
        if (m_Tokenizer.tokenType() == TokenType::TrueValue)
            value = true;
        else if (m_Tokenizer.tokenType() == TokenType::FalseValue)
            value = false;
        else
            return false;
        tokRestorer.clear();
        return true;
    }

    #define IMPLEMENT_READ_VALUE(type, tokenType) \
        bool BufferReader::readValue(type& value) \
        { \
            TokenizerPositionRestorer tokRestorer(m_Tokenizer); \
            if (!readTokenType(TokenType::tokenType) || !readRawValue(value)) \
                return false; \
            tokRestorer.clear(); \
            return true; \
        }

    IMPLEMENT_READ_VALUE(char, CharValue)

    IMPLEMENT_READ_VALUE(int8_t, Int8Value)

    IMPLEMENT_READ_VALUE(uint8_t, UInt8Value)

    IMPLEMENT_READ_VALUE(int16_t, Int16Value)

    IMPLEMENT_READ_VALUE(int32_t, Int32Value)

    IMPLEMENT_READ_VALUE(int64_t, Int64Value)

    IMPLEMENT_READ_VALUE(long, Int32Value)

    IMPLEMENT_READ_VALUE(unsigned, Int32Value)

    IMPLEMENT_READ_VALUE(float, Float32Value)

    IMPLEMENT_READ_VALUE(double, Float64Value)

    IMPLEMENT_READ_VALUE(StringRef, StringValue)

    #define IMPLEMENT_READ_RAW_VALUE(type) \
        bool BufferReader::readRawValue(type& value) \
        { \
            return m_Tokenizer.read(value); \
        }

    IMPLEMENT_READ_RAW_VALUE(char);

    IMPLEMENT_READ_RAW_VALUE(int8_t);

    IMPLEMENT_READ_RAW_VALUE(uint8_t);

    IMPLEMENT_READ_RAW_VALUE(int16_t);

    IMPLEMENT_READ_RAW_VALUE(int32_t);

    IMPLEMENT_READ_RAW_VALUE(int64_t);

    IMPLEMENT_READ_RAW_VALUE(long);

    IMPLEMENT_READ_RAW_VALUE(unsigned);

    IMPLEMENT_READ_RAW_VALUE(float);

    IMPLEMENT_READ_RAW_VALUE(double);

    bool BufferReader::readRawValue(StringRef& stringRef)
    {
        int64_t size;
        if (!readCompatibleValue(size))
            return false;
        stringRef = StringRef((const char*)m_Tokenizer.position(), size_t(size));
        return m_Tokenizer.advance(size_t(size));
    }

    #define IMPLEMENT_CASE_VALUE_TYPE(tokenType) \
        case ValueType::tokenType: \
        { \
            TokenTypeToType<TokenType::tokenType>::Type tmp; \
            if (!m_Tokenizer.read(tmp)) \
                return false; \
            value = (std::remove_reference<decltype(value)>::type)tmp; \
        } \
        break

    bool BufferReader::readCompatibleRawValue(int8_t& value, ValueType type)
    {
        char tmp;
        if (!readCompatibleRawValue(tmp, type))
            return false;
        value = static_cast<int8_t>(tmp);
        return true;
    }

    bool BufferReader::readCompatibleRawValue(uint8_t& value, ValueType type)
    {
        char tmp;
        if (!readCompatibleRawValue(tmp, type))
            return false;
        value = static_cast<uint8_t>(tmp);
        return true;
    }

    bool BufferReader::readCompatibleRawValue(int16_t& value, ValueType type)
    {
        switch (type)
        {
        case ValueType::TrueValue:
            value = 1;
            break;
        case ValueType::FalseValue:
            value = 0;
            break;
        IMPLEMENT_CASE_VALUE_TYPE(Int8Value);
        IMPLEMENT_CASE_VALUE_TYPE(UInt8Value);
        case ValueType::Int16Value:
            return readRawValue(value);
        IMPLEMENT_CASE_VALUE_TYPE(CharValue);
        default:
            return false;
        }
        return true;
    }

    bool BufferReader::readCompatibleRawValue(int32_t& value, ValueType type)
    {
        switch (type)
        {
        case ValueType::TrueValue:
            value = 1;
            break;
        case ValueType::FalseValue:
            value = 0;
            break;
        IMPLEMENT_CASE_VALUE_TYPE(Int8Value);
        IMPLEMENT_CASE_VALUE_TYPE(UInt8Value);
        IMPLEMENT_CASE_VALUE_TYPE(Int16Value);
        case ValueType::Int32Value:
            return readRawValue(value);
        IMPLEMENT_CASE_VALUE_TYPE(CharValue);
        default:
            return false;
        }
        return true;
    }

    bool BufferReader::readCompatibleRawValue(int64_t& value, ValueType type)
    {
        switch (type)
        {
        case ValueType::TrueValue:
            value = 1;
            break;
        case ValueType::FalseValue:
            value = 0;
            break;
        IMPLEMENT_CASE_VALUE_TYPE(Int8Value);
        IMPLEMENT_CASE_VALUE_TYPE(UInt8Value);
        IMPLEMENT_CASE_VALUE_TYPE(Int16Value);
        IMPLEMENT_CASE_VALUE_TYPE(Int32Value);
        case ValueType::Int64Value:
            return readRawValue(value);
        IMPLEMENT_CASE_VALUE_TYPE(CharValue);
        default:
            return false;
        }
        return true;
    }

    bool BufferReader::readCompatibleRawValue(long& value, ValueType type)
    {
        int32_t tmp;
        if (!readCompatibleRawValue(tmp, type))
            return false;
        value = tmp;
        return true;
    }

    bool BufferReader::readCompatibleRawValue(unsigned& value, ValueType type)
    {
        int32_t tmp;
        if (!readCompatibleRawValue(tmp, type))
            return false;
        value = (unsigned)tmp;
        return true;
    }

    bool BufferReader::readCompatibleRawValue(StringRef& stringRef, ValueType type)
    {
        if (type != ValueType::StringValue)
            return false;
        return readRawValue(stringRef);
    }

    bool BufferReader::readCompatibleRawValue(float& value, ValueType type)
    {
        switch (type)
        {
        case ValueType::TrueValue:
            value = 1;
            break;
        case ValueType::FalseValue:
            value = 0;
            break;
        IMPLEMENT_CASE_VALUE_TYPE(Int8Value);
        IMPLEMENT_CASE_VALUE_TYPE(UInt8Value);
        IMPLEMENT_CASE_VALUE_TYPE(Int16Value);
        IMPLEMENT_CASE_VALUE_TYPE(Int32Value);
        IMPLEMENT_CASE_VALUE_TYPE(Int64Value);
        case ValueType::Float32Value:
            return readRawValue(value);
        IMPLEMENT_CASE_VALUE_TYPE(CharValue);
        default:
            return false;
        }
        return true;
    }

    bool BufferReader::readCompatibleRawValue(double& value, ValueType type)
    {
        switch (type)
        {
        case ValueType::TrueValue:
            value = 1;
            break;
        case ValueType::FalseValue:
            value = 0;
            break;
        IMPLEMENT_CASE_VALUE_TYPE(Int8Value);
        IMPLEMENT_CASE_VALUE_TYPE(UInt8Value);
        IMPLEMENT_CASE_VALUE_TYPE(Int16Value);
        IMPLEMENT_CASE_VALUE_TYPE(Int32Value);
        IMPLEMENT_CASE_VALUE_TYPE(Int64Value);
        IMPLEMENT_CASE_VALUE_TYPE(Float32Value);
        case ValueType::Float64Value:
            return readRawValue(value);
        IMPLEMENT_CASE_VALUE_TYPE(CharValue);
        default:
            return false;
        }
        return true;
    }

    bool BufferReader::readCompatibleRawValue(char& value, ValueType type)
    {
        switch (type)
        {
        case ValueType::TrueValue:
            value = 1;
            break;
        case ValueType::FalseValue:
            value = 0;
            break;
        IMPLEMENT_CASE_VALUE_TYPE(Int8Value);
        IMPLEMENT_CASE_VALUE_TYPE(UInt8Value);
        case ValueType::CharValue:
            return readRawValue(value);
        default:
            return false;
        }
        return true;
    }

    bool BufferReader::readContainerInfo(ContainerInfo& info)
    {
        TokenizerPositionRestorer tokRestorer(m_Tokenizer);

        if (!m_Tokenizer.nextToken())
        {
            // Actually an error because this non-optimized container doesn't
            // have an end-character, but I think it's more tidy to let
            // whatever call comes next deal with it.
            info = ContainerInfo();
            return true;
        }

        bool hasType = false;
        if (m_Tokenizer.tokenType() == TokenType::ValueType)
        {
            char tmp;
            if (!readRawValue(tmp))
                return false;
            info.type = static_cast<ValueType>(tmp);
            hasType = true;
            if (!m_Tokenizer.nextToken())
                return false;
        }

        if (m_Tokenizer.tokenType() == TokenType::Count)
        {
            if (!readCompatibleValue(info.length))
                return false;
            tokRestorer.clear();
        }
        else if (hasType)
        {
            return false;
        }
        else
        {
            info.length = -1;
        }

        return true;
    }

    bool BufferReader::readTokenType(TokenType type)
    {
        return m_Tokenizer.nextToken() && m_Tokenizer.tokenType() == type;
    }
}
