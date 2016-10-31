//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2015-02-10 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "ContainerInfo.hpp"
#include "StringRef.hpp"
#include "Tokenizer.hpp"
#include "TokenizerPositionRestorer.hpp"
#include "TokenTypeConversions.hpp"

#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>

namespace Byson
{
    class BYSON_API BufferReader
    {
    public:
        BufferReader(const void* buffer, size_t size);

        ~BufferReader();

        size_t position() const;

        void setPosition(size_t pos);

        bool isEndOfBuffer() const;

        bool readBeginObject(ContainerInfo& info);

        bool readEndObject();

        bool isEndObject() const;

        bool readBeginArray(ContainerInfo& info);

        bool readEndArray();

        bool isEndArray() const;

        template <typename T>
        bool readArray(std::vector<T>& values);

        bool readKey(StringRef& stringRef);

        bool readNull();

        bool readNoOp();

        bool readValue(bool& value);

        bool readValue(char& value);

        bool readValue(int8_t& value);

        bool readValue(uint8_t& value);

        bool readValue(int16_t& value);

        bool readValue(int32_t& value);

        bool readValue(int64_t& value);

        bool readValue(long& value);

        bool readValue(unsigned& value);

        bool readValue(float& value);

        bool readValue(double& value);

        bool readValue(StringRef& stringRef);

        template <typename T>
        bool readCompatibleValue(T& value)
        {
            TokenizerPositionRestorer tokRestorer(m_Tokenizer);
            if (!m_Tokenizer.nextToken())
                return false;
            if (!readCompatibleRawValue(value, ValueType(
                    m_Tokenizer.tokenType())))
                return false;
            tokRestorer.clear();
            return true;
        }

        bool readRawValue(char& value);

        bool readRawValue(int8_t& value);

        bool readRawValue(uint8_t& value);

        bool readRawValue(int16_t& value);

        bool readRawValue(int32_t& value);

        bool readRawValue(int64_t& value);

        bool readRawValue(long& value);

        bool readRawValue(unsigned& value);

        bool readRawValue(float& value);

        bool readRawValue(double& value);

        bool readRawValue(StringRef& stringRef);

        bool readCompatibleRawValue(char& value, ValueType type);

        bool readCompatibleRawValue(int8_t& value, ValueType type);

        bool readCompatibleRawValue(uint8_t& value, ValueType type);

        bool readCompatibleRawValue(int16_t& value, ValueType type);

        bool readCompatibleRawValue(int32_t& value, ValueType type);

        bool readCompatibleRawValue(int64_t& value, ValueType type);

        bool readCompatibleRawValue(long& value, ValueType type);

        bool readCompatibleRawValue(unsigned& value, ValueType type);

        bool readCompatibleRawValue(float& value, ValueType type);

        bool readCompatibleRawValue(double& value, ValueType type);

        bool readCompatibleRawValue(StringRef& stringRef, ValueType type);

    private:
        bool readContainerInfo(ContainerInfo& info);

        bool readTokenType(TokenType type);

        Tokenizer m_Tokenizer;
        const void* m_Buffer;
    };

    template <typename T>
    bool BufferReader::readArray(std::vector<T>& values)
    {
        TokenizerPositionRestorer tokRestorer(m_Tokenizer);

        ContainerInfo info;
        if (!readBeginArray(info))
            return false;

        if (TypeToValueType<T>::valueType == info.type)
        {
            values.resize(size_t(info.length));
            if (!m_Tokenizer.read(values.data(), size_t(info.length)))
                return false;
        }
        else if (info.type != ValueType::UNDEFINED_VALUE)
        {
            values.reserve(size_t(info.length));
            for (auto i = 0LL; i < info.length; ++i)
            {
                T value;
                if (!readCompatibleRawValue(value, info.type))
                    return false;
                values.push_back(value);
            }
        }
        else if (info.length >= 0)
        {
            values.reserve(size_t(info.length));
            for (auto i = 0LL; i < info.length; ++i)
            {
                T value;
                if (!readCompatibleValue(value))
                    return false;
                values.push_back(value);
            }
        }
        else
        {
            while (!isEndArray())
            {
                T value;
                if (!readCompatibleValue(value))
                    return false;
                values.push_back(value);
            }
            if (!readEndArray())
                return false;
        }

        tokRestorer.clear();
        return true;
    }
}
