//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2014-04-24 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Writer.hpp"
#include <iostream>
#include <fstream>

namespace YubJson
{
    namespace
    {
//        inline void reverse(uint8_t& value)
//        {}
//
//        inline void reverse(uint16_t& value)
//        {
//            auto* u8 = reinterpret_cast<char*>(&value);
//            std::swap(u8[0], u8[1]);
//        }
//
//        inline void reverse(uint32_t& value)
//        {
//            auto* u8 = reinterpret_cast<char*>(&value);
//            std::swap(u8[0], u8[3]);
//            std::swap(u8[1], u8[2]);
//        }
//
//        inline void reverse(uint64_t& value)
//        {
//            auto* u8 = reinterpret_cast<char*>(&value);
//            std::swap(u8[0], u8[7]);
//            std::swap(u8[1], u8[6]);
//            std::swap(u8[2], u8[5]);
//            std::swap(u8[3], u8[4]);
//        }

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
        : m_Stream(&std::cout)
    {
        m_States.push(StateFlags::NONE);
    }

    Writer::Writer(const std::string& fileName)
        : m_StreamPtr(new std::ofstream(fileName, std::ios_base::binary)),
          m_Stream(m_StreamPtr.get())
    {
        m_States.push(StateFlags::NONE);
    }

    Writer::Writer(std::ostream& stream)
            : m_Stream(&stream)
    {
        m_States.push(StateFlags::NONE);
    }

    Writer::Writer(std::unique_ptr<std::ostream>&& stream)
        : m_StreamPtr(std::move(stream)),
          m_Stream(m_StreamPtr.get())
    {
        m_States.push(StateFlags::NONE);
    }

    Writer::~Writer()
    {}

    std::ostream* Writer::stream() const
    {
        return m_Stream;
    }

    void Writer::writeBeginObject(int64_t count, ValueType valueType)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_Name);

        m_Stream->put('{');
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

    void Writer::writeBeginObject(const std::string& name,
                                  int64_t count,
                                  ValueType valueType)
    {
        setValueName(m_Name);
        writeBeginObject(count, valueType);
    }

    void Writer::writeEndObject()
    {
        if (m_States.top() & StateFlags::WRITE_END_STRUCTURE)
            m_Stream->put('}');
        m_States.pop();
    }

    void Writer::writeBeginArray(int64_t count, ValueType valueType)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_Name);

        m_Stream->put('[');
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

    void Writer::writeBeginArray(const std::string& name, int64_t count,
                                 ValueType valueType)
    {
        setValueName(name);
        writeBeginArray(count, valueType);
    }

    void Writer::writeEndArray()
    {
        if (m_States.top() & StateFlags::WRITE_END_STRUCTURE)
            m_Buffer.push_back('}');
        m_States.pop();
    }

    void Writer::setValueName(const std::string& name)
    {
        m_Name = name;
    }

    void Writer::writeNull()
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_Name);
        if (m_States.top() & StateFlags::WRITE_TYPE)
            m_Stream->put(uint8_t(ValueType::NullValue));
    }

    void Writer::writeNull(const std::string& name)
    {
        setValueName(name);
        writeNull();
    }

    void Writer::writeNoOp()
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_Name);
        if (m_States.top() & StateFlags::WRITE_TYPE)
            m_Stream->put('N');
    }

    void Writer::writeBool(bool value)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_Name);
        if (m_States.top() & StateFlags::WRITE_TYPE)
            m_Stream->put(value ? 'T' : 'F');
    }

    void Writer::writeBool(const std::string& name, bool value)
    {
        setValueName(name);
        writeBool(value);
    }

    void Writer::writeValue(int8_t value)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_Name);
        if (m_States.top() & StateFlags::WRITE_TYPE)
            m_Stream->put(uint8_t(ValueType::Int8Value));
        writeRawValue(value);
    }

    void Writer::writeValue(uint8_t value)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_Name);
        if (m_States.top() & StateFlags::WRITE_TYPE)
            m_Stream->put(uint8_t(ValueType::UInt8Value));
        writeRawValue(value);
    }

    void Writer::writeValue(int16_t value)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_Name);
        if (m_States.top() & StateFlags::WRITE_TYPE)
            m_Stream->put(uint8_t(ValueType::Int16Value));
        writeRawValue(value);
    }

    void Writer::writeValue(uint16_t value)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_Name);
        if (m_States.top() & StateFlags::WRITE_TYPE)
            m_Stream->put(uint8_t(ValueType::Int16Value));
        writeRawValue(int16_t(value));
    }

    void Writer::writeValue(int32_t value)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_Name);
        if (m_States.top() & StateFlags::WRITE_TYPE)
            m_Stream->put(uint8_t(ValueType::Int32Value));
        writeRawValue(value);
    }

    void Writer::writeValue(uint32_t value)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_Name);
        if (m_States.top() & StateFlags::WRITE_TYPE)
            m_Stream->put(uint8_t(ValueType::Int32Value));
        writeRawValue(int32_t(value));
    }

    void Writer::writeValue(int64_t value)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_Name);
        if (m_States.top() & StateFlags::WRITE_TYPE)
            m_Stream->put(uint8_t(ValueType::Int64Value));
        writeRawValue(value);
    }

    void Writer::writeValue(uint64_t value)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_Name);
        if (m_States.top() & StateFlags::WRITE_TYPE)
            m_Stream->put(uint8_t(ValueType::Int64Value));
        writeRawValue(int64_t(value));
    }

    void Writer::writeValue(float value)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_Name);
        if (m_States.top() & StateFlags::WRITE_TYPE)
            m_Stream->put(uint8_t(ValueType::Float32Value));
        writeRawValue(value);
    }

    void Writer::writeValue(double value)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_Name);
        if (m_States.top() & StateFlags::WRITE_TYPE)
            m_Stream->put(uint8_t(ValueType::Float64Value));
        writeRawValue(value);
    }

    void Writer::writeValue(char value)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_Name);
        if (m_States.top() & StateFlags::WRITE_TYPE)
            m_Stream->put(uint8_t(ValueType::CharValue));
        writeRawValue(value);
    }

    void Writer::writeValue(const std::string& value)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_Name);
        if (m_States.top() & StateFlags::WRITE_TYPE)
            m_Stream->put(uint8_t(ValueType::StringValue));
        writeRawValue(value);
    }

    void Writer::writeShortestValue(int64_t value)
    {
        if (m_States.top() & StateFlags::WRITE_NAME)
            writeRawValue(m_Name);
        writeShortestRawValue(value);
    }

    void Writer::writeShortestValue(const std::string& name, int64_t value)
    {
        setValueName(name);
        writeShortestValue(value);
    }

    void Writer::writeRawValue(int8_t value)
    {
        m_Stream->put(char(value));
    }

    void Writer::writeRawValue(uint8_t value)
    {
        m_Stream->put(char(value));
    }

    void Writer::writeRawValue(char value)
    {
        m_Stream->put(value);
    }

    template <typename T>
    void writeImpl(std::ostream& stream, T rawValue)
    {
        auto value = bigEndian(rawValue);
        stream.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }

    void Writer::writeRawValue(int16_t value)
    {
        writeImpl(*m_Stream, value);
    }

    void Writer::writeRawValue(int32_t value)
    {
        writeImpl(*m_Stream, value);
    }

    void Writer::writeRawValue(int64_t value)
    {
        writeImpl(*m_Stream, value);
    }

    void Writer::writeRawValue(float value)
    {
        writeImpl(*m_Stream, value);
    }

    void Writer::writeRawValue(double value)
    {
        writeImpl(*m_Stream, value);
    }

    void Writer::writeRawValue(const std::string& value)
    {
        writeShortestRawValue(int64_t(value.size()));
        m_Stream->write(value.data(), value.size());
    }

    void Writer::writeRawValues(const char* values, size_t count)
    {
        m_Stream->write(values, count);
    }

    void Writer::writeRawValues(const int8_t* values, size_t count)
    {
        m_Stream->write(reinterpret_cast<const char*>(values), count);
    }

    void Writer::writeRawValues(const uint8_t* values, size_t count)
    {
        m_Stream->write(reinterpret_cast<const char*>(values), count);
    }

    void Writer::writeShortestRawValue(int64_t value)
    {
        if (value >= -0x80LL)
        {
            if (value < 0x80LL)
            {
                m_Stream->put(uint8_t(ValueType::Int8Value));
                writeRawValue(static_cast<int8_t>(value));
            }
            else if (value < 0x100LL)
            {
                m_Stream->put(uint8_t(ValueType::UInt8Value));
                writeRawValue(static_cast<uint8_t>(value));
            }
            else if (value < 0x8000LL)
            {
                m_Stream->put(uint8_t(ValueType::Int16Value));
                writeRawValue(static_cast<int16_t>(value));
            }
            else if (value < 0x80000000LL)
            {
                m_Stream->put(uint8_t(ValueType::Int32Value));
                writeRawValue(static_cast<int32_t>(value));
            }
            else
            {
                m_Stream->put(uint8_t(ValueType::Int64Value));
                writeRawValue(value);
            }
        }
        else if (value >= -0x8000LL)
        {
            m_Stream->put(uint8_t(ValueType::Int16Value));
            writeRawValue(static_cast<int16_t>(value));
        }
        else if (value >= -0x80000000LL)
        {
            m_Stream->put(uint8_t(ValueType::Int32Value));
            writeRawValue(static_cast<int32_t>(value));
        }
        else
        {
            m_Stream->put(uint8_t(ValueType::Int64Value));
            writeRawValue(value);
        }
    }

    void Writer::writeValueType(ValueType valueType)
    {
        char str[2] = {'$', char(valueType)};
        m_Stream->write(str, 2);
    }

    void Writer::writeCount(int64_t count)
    {
        m_Stream->put('#');
        writeShortestRawValue(int64_t(count));
    }
}
