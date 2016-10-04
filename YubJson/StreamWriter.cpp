//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2014-04-24 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "StreamWriter.hpp"
#include <iostream>
#include <fstream>

namespace YubJson
{
    namespace
    {
        template <typename T>
        void writeImpl(std::ostream& stream, T rawValue)
        {
            auto value = bigEndian(rawValue);
            stream.write(reinterpret_cast<const char*>(&value), sizeof(value));
        }

        template <typename T>
        void append(std::vector<char>& vec, T value)
        {
            vec.resize(vec.size() + sizeof(T));
            auto dst = vec.end();
            auto alias = reinterpret_cast<const char*>(&value);
            for (auto i = 0; i < sizeof(T); ++i)
                *--dst = alias[i];
        }
    }

    StreamWriter::StreamWriter()
        : m_Stream(&std::cout)
    {
        m_States.push(StateFlags::DEFAULT);
    }

    StreamWriter::StreamWriter(const std::string& fileName)
        : m_StreamPtr(new std::ofstream(fileName, std::ios_base::binary)),
          m_Stream(m_StreamPtr.get())
    {
        m_States.push(StateFlags::DEFAULT);
    }

    StreamWriter::StreamWriter(std::ostream& stream)
            : m_Stream(&stream)
    {
        m_States.push(StateFlags::DEFAULT);
    }

    StreamWriter::StreamWriter(std::unique_ptr<std::ostream>&& stream)
        : m_StreamPtr(std::move(stream)),
          m_Stream(m_StreamPtr.get())
    {
        m_States.push(StateFlags::DEFAULT);
    }

    StreamWriter::~StreamWriter()
    {}

    std::ostream* StreamWriter::stream() const
    {
        return m_Stream;
    }

    void StreamWriter::writeBeginObject(int64_t count, ValueType valueType)
    {
        writePrefix(ValueType::ObjectValue);

        int newState = StateFlags::WRITE_NAME;
        if (count >= 0)
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

    void StreamWriter::writeBeginObject(const std::string& name,
                                        int64_t count,
                                        ValueType valueType)
    {
        setValueName(m_ValueName);
        writeBeginObject(count, valueType);
    }

    void StreamWriter::writeEndObject()
    {
        if (m_States.top() & StateFlags::WRITE_END_STRUCTURE)
            m_Stream->put('}');
        m_States.pop();
    }

    void StreamWriter::writeBeginArray(int64_t count, ValueType valueType)
    {
        writePrefix(ValueType::ArrayValue);

        int newState = StateFlags::NONE;
        if (count >= 0)
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

    void StreamWriter::writeBeginArray(const std::string& name,
                                       int64_t count,
                                       ValueType valueType)
    {
        setValueName(name);
        writeBeginArray(count, valueType);
    }

    void StreamWriter::writeEndArray()
    {
        if (m_States.top() & StateFlags::WRITE_END_STRUCTURE)
            m_Buffer.push_back('}');
        m_States.pop();
    }

    void StreamWriter::setValueName(const std::string& name)
    {
        m_ValueName = name;
    }

    void StreamWriter::writeNull()
    {
        writePrefix(ValueType::NullValue);
    }

    void StreamWriter::writeNull(const std::string& name)
    {
        setValueName(name);
        writeNull();
    }

    void StreamWriter::writeNoOp()
    {
        writePrefix(ValueType::NoOpValue);
    }

    void StreamWriter::writeBool(bool value)
    {
        writePrefix(value ? ValueType::TrueValue : ValueType::FalseValue);
    }

    void StreamWriter::writeBool(const std::string& name, bool value)
    {
        setValueName(name);
        writeBool(value);
    }

    void StreamWriter::writeValue(int8_t value)
    {
        writePrefix(ValueType::Int8Value);
        writeRawValue(value);
    }

    void StreamWriter::writeValue(uint8_t value)
    {
        writePrefix(ValueType::UInt8Value);
        writeRawValue(value);
    }

    void StreamWriter::writeValue(int16_t value)
    {
        writePrefix(ValueType::Int16Value);
        writeRawValue(value);
    }

    void StreamWriter::writeValue(uint16_t value)
    {
        writePrefix(ValueType::Int16Value);
        writeRawValue(int16_t(value));
    }

    void StreamWriter::writeValue(int32_t value)
    {
        writePrefix(ValueType::Int32Value);
        writeRawValue(value);
    }

    void StreamWriter::writeValue(uint32_t value)
    {
        writePrefix(ValueType::Int32Value);
        writeRawValue(int32_t(value));
    }

    void StreamWriter::writeValue(int64_t value)
    {
        writePrefix(ValueType::Int64Value);
        writeRawValue(value);
    }

    void StreamWriter::writeValue(uint64_t value)
    {
        writePrefix(ValueType::Int64Value);
        writeRawValue(int64_t(value));
    }

    void StreamWriter::writeValue(float value)
    {
        writePrefix(ValueType::Float32Value);
        writeRawValue(value);
    }

    void StreamWriter::writeValue(double value)
    {
        writePrefix(ValueType::Float64Value);
        writeRawValue(value);
    }

    void StreamWriter::writeValue(char value)
    {
        writePrefix(ValueType::CharValue);
        writeRawValue(value);
    }

    void StreamWriter::writeValue(const std::string& value)
    {
        writePrefix(ValueType::StringValue);
        writeRawValue(value);
    }

    void StreamWriter::writeShortestValue(int64_t value)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_ValueName);
        writeShortestRawValue(value);
    }

    void StreamWriter::writeShortestValue(const std::string& name, int64_t value)
    {
        setValueName(name);
        writeShortestValue(value);
    }

    void StreamWriter::writeRawValue(int8_t value)
    {
        m_Stream->put(char(value));
    }

    void StreamWriter::writeRawValue(uint8_t value)
    {
        m_Stream->put(char(value));
    }

    void StreamWriter::writeRawValue(char value)
    {
        m_Stream->put(value);
    }

    void StreamWriter::writeRawValue(int16_t value)
    {
        writeImpl(*m_Stream, value);
    }

    void StreamWriter::writeRawValue(int32_t value)
    {
        writeImpl(*m_Stream, value);
    }

    void StreamWriter::writeRawValue(int64_t value)
    {
        writeImpl(*m_Stream, value);
    }

    void StreamWriter::writeRawValue(float value)
    {
        writeImpl(*m_Stream, value);
    }

    void StreamWriter::writeRawValue(double value)
    {
        writeImpl(*m_Stream, value);
    }

    void StreamWriter::writeRawValue(const std::string& value)
    {
        writeShortestRawValue(int64_t(value.size()));
        m_Stream->write(value.data(), value.size());
    }

    void StreamWriter::writeRawValues(const char* values, size_t count)
    {
        m_Stream->write(values, count);
    }

    void StreamWriter::writeRawValues(const int8_t* values, size_t count)
    {
        writeRawValues(reinterpret_cast<const char*>(values), count);
    }

    void StreamWriter::writeRawValues(const uint8_t* values, size_t count)
    {
        writeRawValues(reinterpret_cast<const char*>(values), count);
    }

    void StreamWriter::writeShortestRawValue(int64_t value)
    {
        if (value >= -0x80LL)
        {
            if (value < 0x80LL)
            {
                m_Stream->put(char(ValueType::Int8Value));
                writeRawValue(static_cast<int8_t>(value));
            }
            else if (value < 0x100LL)
            {
                m_Stream->put(char(ValueType::UInt8Value));
                writeRawValue(static_cast<uint8_t>(value));
            }
            else if (value < 0x8000LL)
            {
                m_Stream->put(char(ValueType::Int16Value));
                writeRawValue(static_cast<int16_t>(value));
            }
            else if (value < 0x80000000LL)
            {
                m_Stream->put(char(ValueType::Int32Value));
                writeRawValue(static_cast<int32_t>(value));
            }
            else
            {
                m_Stream->put(char(ValueType::Int64Value));
                writeRawValue(value);
            }
        }
        else if (value >= -0x8000LL)
        {
            m_Stream->put(char(ValueType::Int16Value));
            writeRawValue(static_cast<int16_t>(value));
        }
        else if (value >= -0x80000000LL)
        {
            m_Stream->put(char(ValueType::Int32Value));
            writeRawValue(static_cast<int32_t>(value));
        }
        else
        {
            m_Stream->put(char(ValueType::Int64Value));
            writeRawValue(value);
        }
    }

    void StreamWriter::writePrefix(ValueType valueType)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_ValueName);
        if (m_States.top() & StateFlags::WRITE_TYPE)
            m_Stream->put(char(valueType));
    }

    void StreamWriter::writeValueType(ValueType valueType)
    {
        char str[2] = {'$', char(valueType)};
        m_Stream->write(str, 2);
    }

    void StreamWriter::writeCount(int64_t count)
    {
        m_Stream->put('#');
        writeShortestRawValue(int64_t(count));
    }
}
