//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2014-04-24 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "BufferWriter.hpp"

namespace Byson
{
    namespace
    {
        template <typename T>
        void append(std::vector<uint8_t>& vec, T rawValue)
        {
            auto value = bigEndian(rawValue);
            auto alias = reinterpret_cast<const uint8_t*>(&value);
            vec.insert(vec.end(), alias, alias + sizeof(T));
        }

        void append(std::vector<uint8_t>& vec, const std::string& value)
        {
            vec.insert(end(vec), begin(value), end(value));
        }
    }

    BufferWriter::BufferWriter()
    {
        m_States.push(StateFlags::DEFAULT);
    }

    BufferWriter::BufferWriter(std::vector<uint8_t>&& buffer)
        : m_Buffer(std::move(buffer))
    {
        m_States.push(StateFlags::DEFAULT);
    }

    BufferWriter::~BufferWriter()
    {}

    void BufferWriter::clear()
    {
        m_Buffer.clear();
    }

    const void* BufferWriter::buffer() const
    {
        return &m_Buffer[0];
    }

    size_t BufferWriter::size() const
    {
        return m_Buffer.size();
    }

    void BufferWriter::writeBeginObject(int64_t count, ValueType valueType)
    {
        writePrefix(ValueType::ObjectValue);

        int newState = StateFlags::WRITE_NAME;
        if (count >= 0 && (m_States.top() & WRITE_END_STRUCTURE))
        {
            if (valueType != ValueType::UndefinedValue)
                writeValueType(valueType);
            else
                newState |= StateFlags::WRITE_TYPE;
            writeCount(count);
        }
        else
        {
            newState |= StateFlags::WRITE_TYPE | StateFlags::WRITE_END_STRUCTURE;
        }
        m_States.push(StateFlags(newState));
    }

    void BufferWriter::writeBeginObject(const std::string& name,
                                        int64_t count,
                                        ValueType valueType)
    {
        setValueName(m_ValueName);
        writeBeginObject(count, valueType);
    }

    void BufferWriter::writeEndObject()
    {
        if (m_States.top() & StateFlags::WRITE_END_STRUCTURE)
            m_Buffer.push_back('}');
        m_States.pop();
    }

    void BufferWriter::writeBeginArray(int64_t count, ValueType valueType)
    {
        writePrefix(ValueType::ArrayValue);

        int newState = StateFlags::NONE;
        if (count >= 0 && (m_States.top() & WRITE_END_STRUCTURE))
        {
            if (valueType != ValueType::UndefinedValue)
                writeValueType(valueType);
            else
                newState |= StateFlags::WRITE_TYPE;
            writeCount(count);
        }
        else
        {
            newState |= StateFlags::WRITE_TYPE | StateFlags::WRITE_END_STRUCTURE;
        }
        m_States.push(StateFlags(newState));
    }

    void BufferWriter::writeBeginArray(const std::string& name,
                                       int64_t count,
                                       ValueType valueType)
    {
        setValueName(name);
        writeBeginArray(count, valueType);
    }

    void BufferWriter::writeEndArray()
    {
        if (m_States.top() & StateFlags::WRITE_END_STRUCTURE)
            m_Buffer.push_back(']');
        m_States.pop();
    }

    void BufferWriter::setValueName(const std::string& name)
    {
        m_ValueName = name;
    }

    void BufferWriter::writeNull()
    {
        writePrefix(ValueType::NullValue);
    }

    void BufferWriter::writeNull(const std::string& name)
    {
        setValueName(name);
        writeNull();
    }

    void BufferWriter::writeNoOp()
    {
        writePrefix(ValueType::NoOpValue);
    }

    void BufferWriter::writeBool(bool value)
    {
        writePrefix(value ? ValueType::TrueValue : ValueType::FalseValue);
    }

    void BufferWriter::writeBool(const std::string& name, bool value)
    {
        setValueName(name);
        writeBool(value);
    }

    void BufferWriter::writeValue(int8_t value)
    {
        writePrefix(ValueType::Int8Value);
        writeRawValue(value);
    }

    void BufferWriter::writeValue(uint8_t value)
    {
        writePrefix(ValueType::UInt8Value);
        writeRawValue(value);
    }

    void BufferWriter::writeValue(int16_t value)
    {
        writePrefix(ValueType::Int16Value);
        writeRawValue(value);
    }

    void BufferWriter::writeValue(uint16_t value)
    {
        writePrefix(ValueType::Int16Value);
        writeRawValue(int16_t(value));
    }

    void BufferWriter::writeValue(int32_t value)
    {
        writePrefix(ValueType::Int32Value);
        writeRawValue(value);
    }

    void BufferWriter::writeValue(uint32_t value)
    {
        writePrefix(ValueType::Int32Value);
        writeRawValue(int32_t(value));
    }

    void BufferWriter::writeValue(int64_t value)
    {
        writePrefix(ValueType::Int64Value);
        writeRawValue(value);
    }

    void BufferWriter::writeValue(uint64_t value)
    {
        writePrefix(ValueType::Int64Value);
        writeRawValue(int64_t(value));
    }

    void BufferWriter::writeValue(float value)
    {
        writePrefix(ValueType::Float32Value);
        writeRawValue(value);
    }

    void BufferWriter::writeValue(double value)
    {
        writePrefix(ValueType::Float64Value);
        writeRawValue(value);
    }

    void BufferWriter::writeValue(char value)
    {
        writePrefix(ValueType::CharValue);
        writeRawValue(value);
    }

    void BufferWriter::writeValue(const std::string& value)
    {
        writePrefix(ValueType::StringValue);
        writeRawValue(value);
    }

    void BufferWriter::writeShortestValue(int64_t value)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_ValueName);
        writeShortestRawValue(value);
    }

    void BufferWriter::writeShortestValue(const std::string& name, int64_t value)
    {
        setValueName(name);
        writeShortestValue(value);
    }

    void BufferWriter::writeRawValue(int8_t value)
    {
        m_Buffer.push_back(uint8_t(value));
    }

    void BufferWriter::writeRawValue(uint8_t value)
    {
        m_Buffer.push_back(value);
    }

    void BufferWriter::writeRawValue(char value)
    {
        m_Buffer.push_back(uint8_t(value));
    }

    void BufferWriter::writeRawValue(int16_t value)
    {
        append(m_Buffer, value);
    }

    void BufferWriter::writeRawValue(int32_t value)
    {
        append(m_Buffer, value);
    }

    void BufferWriter::writeRawValue(int64_t value)
    {
        append(m_Buffer, value);
    }

    void BufferWriter::writeRawValue(float value)
    {
        append(m_Buffer, value);
    }

    void BufferWriter::writeRawValue(double value)
    {
        append(m_Buffer, value);
    }

    void BufferWriter::writeRawValue(const std::string& value)
    {
        writeShortestRawValue(int64_t(value.size()));
        append(m_Buffer, value);
    }

    void BufferWriter::writeRawValues(const char* values, size_t count)
    {
        writeRawValues(reinterpret_cast<const uint8_t*>(values), count);
    }

    void BufferWriter::writeRawValues(const int8_t* values, size_t count)
    {
        writeRawValues(reinterpret_cast<const uint8_t*>(values), count);
    }

    void BufferWriter::writeRawValues(const uint8_t* values, size_t count)
    {
        m_Buffer.insert(end(m_Buffer), values, values + count);
    }

    void BufferWriter::writeShortestRawValue(int64_t value)
    {
        if (value >= -0x80LL)
        {
            if (value < 0x80LL)
            {
                m_Buffer.push_back(uint8_t(ValueType::Int8Value));
                writeRawValue(static_cast<int8_t>(value));
            }
            else if (value < 0x100LL)
            {
                m_Buffer.push_back(uint8_t(ValueType::UInt8Value));
                writeRawValue(static_cast<uint8_t>(value));
            }
            else if (value < 0x8000LL)
            {
                m_Buffer.push_back(uint8_t(ValueType::Int16Value));
                writeRawValue(static_cast<int16_t>(value));
            }
            else if (value < 0x80000000LL)
            {
                m_Buffer.push_back(uint8_t(ValueType::Int32Value));
                writeRawValue(static_cast<int32_t>(value));
            }
            else
            {
                m_Buffer.push_back(uint8_t(ValueType::Int64Value));
                writeRawValue(value);
            }
        }
        else if (value >= -0x8000LL)
        {
            m_Buffer.push_back(uint8_t(ValueType::Int16Value));
            writeRawValue(static_cast<int16_t>(value));
        }
        else if (value >= -0x80000000LL)
        {
            m_Buffer.push_back(uint8_t(ValueType::Int32Value));
            writeRawValue(static_cast<int32_t>(value));
        }
        else
        {
            m_Buffer.push_back(uint8_t(ValueType::Int64Value));
            writeRawValue(value);
        }
    }

    std::vector<uint8_t>& BufferWriter::vector()
    {
        return m_Buffer;
    }

    void BufferWriter::writePrefix(ValueType valueType)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_ValueName);
        if (m_States.top() & StateFlags::WRITE_TYPE)
            m_Buffer.push_back(uint8_t(valueType));
    }

    void BufferWriter::writeValueType(ValueType valueType)
    {
        m_Buffer.push_back('$');
        m_Buffer.push_back(uint8_t(valueType));
    }

    void BufferWriter::writeCount(int64_t count)
    {
        m_Buffer.push_back('#');
        writeShortestValue(int64_t(count));
    }
}
