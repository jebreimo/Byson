//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2014-04-24 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Writer.hpp"
#include <string>

namespace YubJson
{
    namespace
    {
        template <typename T>
        void append(std::vector<uint8_t>& vec, T value)
        {
            vec.resize(vec.size() + sizeof(T));
            auto dst = vec.end();
            auto alias = reinterpret_cast<const uint8_t*>(&value);
            for (auto i = 0; i < sizeof(T); ++i)
                *--dst = alias[i];
        }
    }

    Writer::Writer()
    {}

    Writer::~Writer()
    {}

    void Writer::clear()
    {
        m_Buffer.clear();
    }

    const void* Writer::buffer() const
    {
        return &m_Buffer[0];
    }

    size_t Writer::size() const
    {
        return m_Buffer.size();
    }

    void Writer::beginObject()
    {
        m_Buffer.push_back('{');
    }

    void Writer::beginOptimizedObject(size_t count, ValueType valueType)
    {
        m_Buffer.push_back('{');
        if (valueType != ValueType::UndefinedValue)
            writeValueType(valueType);
        writeCount(count);
    }

    void Writer::endObject()
    {
        m_Buffer.push_back('}');
    }

    void Writer::beginArray()
    {
        m_Buffer.push_back('[');
    }

    void Writer::beginOptimizedArray(size_t count, ValueType valueType)
    {
        m_Buffer.push_back('[');
        if (valueType != ValueType::UndefinedValue)
            writeValueType(valueType);
        writeCount(count);
    }

    void Writer::endArray()
    {
        m_Buffer.push_back(']');
    }

    void Writer::key(const char* key, size_t length)
    {
        writeRaw(key, length);
    }

    void Writer::key(const std::string& key)
    {
        writeRaw(key);
    }

    void Writer::writeNull()
    {
        m_Buffer.push_back(uint8_t(ValueType::NullValue));
    }

    void Writer::writeNoOp()
    {
        m_Buffer.push_back('N');
    }

    void Writer::write(bool value)
    {
        m_Buffer.push_back(value ? 'T' : 'F');
    }

    void Writer::write(int8_t value)
    {
        m_Buffer.push_back(uint8_t(ValueType::Int8Value));
        writeRaw(value);
    }

    void Writer::write(uint8_t value)
    {
        m_Buffer.push_back(uint8_t(ValueType::UInt8Value));
        writeRaw(value);
    }

    void Writer::write(int16_t value)
    {
        m_Buffer.push_back(uint8_t(ValueType::Int16Value));
        writeRaw(value);
    }

    void Writer::write(uint16_t value)
    {
        m_Buffer.push_back(uint8_t(ValueType::Int16Value));
        writeRaw(int16_t(value));
    }

    void Writer::write(int32_t value)
    {
        m_Buffer.push_back(uint8_t(ValueType::Int32Value));
        writeRaw(value);
    }

    void Writer::write(uint32_t value)
    {
        m_Buffer.push_back(uint8_t(ValueType::Int32Value));
        writeRaw(int32_t(value));
    }

    void Writer::write(int64_t value)
    {
        m_Buffer.push_back(uint8_t(ValueType::Int64Value));
        writeRaw(value);
    }

    void Writer::write(uint64_t value)
    {
        m_Buffer.push_back(uint8_t(ValueType::Int64Value));
        writeRaw(int64_t(value));
    }

    void Writer::write(float value)
    {
        m_Buffer.push_back(uint8_t(ValueType::Float32Value));
        writeRaw(value);
    }

    void Writer::write(double value)
    {
        m_Buffer.push_back(uint8_t(ValueType::Float64Value));
        writeRaw(value);
    }

    void Writer::write(char value)
    {
        m_Buffer.push_back(uint8_t(ValueType::CharValue));
        writeRawChar(value);
    }

    void Writer::write(const char* value, size_t length)
    {
        m_Buffer.push_back(uint8_t(ValueType::StringValue));
        writeRaw(value, length);
    }

    void Writer::write(const std::string& value)
    {
        write(value.c_str(), value.size());
    }

    void Writer::writeShortest(int64_t value)
    {
        if (value >= -0x80ll)
        {
            if (value < 0x80ll)
                write(static_cast<int8_t>(value));
            else if (value < 0x100ll)
                write(static_cast<uint8_t>(value));
            else if (value < 0x8000ll)
                write(static_cast<int16_t>(value));
            else if (value < 0x80000000ll)
                write(static_cast<int32_t>(value));
            else
                write(value);
        }
        else if (value >= -0x8000ll)
            write(static_cast<int16_t>(value));
        else if (value >= -0x80000000ll)
            write(static_cast<int32_t>(value));
        else
            write(value);
    }

    void Writer::writeRaw(int8_t value)
    {
        m_Buffer.push_back(uint8_t(value));
    }

    void Writer::writeRaw(uint8_t value)
    {
        m_Buffer.push_back(value);
    }

    void Writer::writeRaw(int16_t value)
    {
        append(m_Buffer, value);
    }

    void Writer::writeRaw(int32_t value)
    {
        append(m_Buffer, value);
    }

    void Writer::writeRaw(int64_t value)
    {
        append(m_Buffer, value);
    }

    void Writer::writeRaw(float value)
    {
        append(m_Buffer, value);
    }

    void Writer::writeRaw(double value)
    {
        append(m_Buffer, value);
    }

    void Writer::writeRawChar(char value)
    {
        m_Buffer.push_back(uint8_t(value));
    }

    void Writer::writeRaw(const char* value, size_t length)
    {
        writeShortest(int64_t(length));
        m_Buffer.insert(end(m_Buffer), value, value + length);
    }

    void Writer::writeRaw(const std::string& value)
    {
        writeRaw(value.c_str(), value.size());
    }

    void Writer::writeValueType(ValueType valueType)
    {
        m_Buffer.push_back('$');
        m_Buffer.push_back(uint8_t(valueType));
    }

    void Writer::writeCount(size_t count)
    {
        m_Buffer.push_back('#');
        writeShortest(int64_t(count));
    }
}
