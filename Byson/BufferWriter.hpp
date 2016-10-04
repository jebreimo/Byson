//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2014-04-24 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cstdint>
#include <stack>
#include <vector>
#include "BysonDefinitions.hpp"
#include "Endian.hpp"
#include "ValueType.hpp"

namespace Byson
{
    class BYSON_API BufferWriter
    {
    public:
        BufferWriter();

        BufferWriter(std::vector<uint8_t>&& buffer);

        ~BufferWriter();

        void clear();

        const void* buffer() const;

        size_t size() const;

        void writeBeginObject(int64_t count = -1,
                              ValueType valueType = ValueType::UndefinedValue);

        void writeBeginObject(const std::string& name,
                              int64_t count = -1,
                              ValueType valueType = ValueType::UndefinedValue);

        void writeEndObject();

        void writeBeginArray(int64_t count = -1,
                             ValueType valueType = ValueType::UndefinedValue);

        void writeBeginArray(const std::string& name,
                             int64_t count = -1,
                             ValueType valueType = ValueType::UndefinedValue);

        void writeEndArray();

        void setValueName(const std::string& name);

        void writeNull();

        void writeNull(const std::string& name);

        void writeNoOp();

        void writeBool(bool value);

        void writeBool(const std::string& name, bool value);

        void writeValue(int8_t value);

        void writeValue(uint8_t value);

        void writeValue(int16_t value);

        void writeValue(uint16_t value);

        void writeValue(int32_t value);

        void writeValue(uint32_t value);

        void writeValue(int64_t value);

        void writeValue(uint64_t value);

        void writeValue(float value);

        void writeValue(double value);

        void writeValue(char value);

        void writeValue(const std::string& value);

        template <typename T>
        void writeValue(const std::string& name, T&& value)
        {
            setValueName(name);
            writeValue(std::forward<T>(value));
        }

        void writeShortestValue(int64_t value);

        void writeShortestValue(const std::string& name, int64_t value);

        void writeRawValue(int8_t value);

        void writeRawValue(uint8_t value);

        void writeRawValue(char value);

        void writeRawValue(int16_t value);

        void writeRawValue(int32_t value);

        void writeRawValue(int64_t value);

        void writeRawValue(float value);

        void writeRawValue(double value);

        void writeRawValue(const std::string& value);

        template <typename T>
        void writeRawValues(const T* values, size_t count)
        {
            auto bufSize = m_Buffer.size();
            m_Buffer.reserve(bufSize + count * sizeof(T));
            for (size_t i = 0; i < count; ++i)
            {
                auto value = bigEndian(values[i]);
                auto alias = reinterpret_cast<const uint8_t*>(&value);
                m_Buffer.insert(end(m_Buffer), alias, alias + sizeof(T));
            }
        }

        void writeRawValues(const char* values, size_t count);

        void writeRawValues(const int8_t* values, size_t count);

        void writeRawValues(const uint8_t* values, size_t count);

        void writeShortestRawValue(int64_t value);

        std::vector<uint8_t>& vector();

    private:
        void writePrefix(ValueType valueType);

        void writeValueType(ValueType valueType);

        void writeCount(int64_t count);

        enum StateFlags
        {
            NONE = 0,
            WRITE_NAME = 1,
            WRITE_TYPE = 2,
            WRITE_END_STRUCTURE = 4,
            DEFAULT = 6
        };

        #ifdef _MSC_VER
            #pragma warning(push)
            #pragma warning(disable: 4251)
        #endif
        std::stack<StateFlags> m_States;
        std::string m_ValueName;
        std::vector<uint8_t> m_Buffer;
        #ifdef _MSC_VER
            #pragma warning(pop)
        #endif
    };
}
