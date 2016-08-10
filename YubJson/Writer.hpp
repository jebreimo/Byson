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
#include "Endian.hpp"
#include "ValueType.hpp"
#include "YubJsonDefinitions.hpp"

namespace YubJson
{
    class UBJSON_API Writer
    {
    public:
        Writer();

        Writer(const std::string& fileName);

        Writer(std::ostream& stream);

        Writer(std::unique_ptr<std::ostream>&& stream);

        ~Writer();

        std::ostream* stream() const;

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
            writeValue(value);
        }

        void writeShortestValue(int64_t value);

        void writeShortestValue(const std::string& name, int64_t value);

        void writeRawValue(int8_t value);

        void writeRawValue(uint8_t value);

        void writeRawValue(int16_t value);

        void writeRawValue(int32_t value);

        void writeRawValue(int64_t value);

        void writeRawValue(float value);

        void writeRawValue(double value);

        void writeRawValue(char value);

        void writeRawValue(const std::string& value);

        void writeShortestRawValue(int64_t value);

        template <typename T>
        void writeRawValues(const T* values, size_t count)
        {
            if (count == 0)
                return;

            auto nValuesPerBuffer = BUFFER_SIZE / sizeof(T);
            auto nFullBuffers = count / nValuesPerBuffer;
            if (nFullBuffers > 0)
            {
                m_Buffer.resize(nValuesPerBuffer * sizeof(T));
                for (size_t i = 0; i < nFullBuffers; ++i)
                {
                    copyBigEndianValues(values, nValuesPerBuffer,
                                        m_Buffer.data());
                    writeRawValues(m_Buffer.data(), m_Buffer.size());
                }
            }

            auto nValuesInFinalBuffer = count % nValuesPerBuffer;
            if (nValuesInFinalBuffer > 0)
            {
                m_Buffer.resize(nValuesInFinalBuffer * sizeof(T));
                copyBigEndianValues(values, nValuesInFinalBuffer,
                                    m_Buffer.data());
                writeRawValues(m_Buffer.data(), m_Buffer.size());
            }
        }

        void writeRawValues(const char* values, size_t count);

        void writeRawValues(const int8_t* values, size_t count);

        void writeRawValues(const uint8_t* values, size_t count);
    private:
        void writeValueType(ValueType valueType);

        void writeCount(int64_t count);

        enum StateFlags
        {
            NONE = 0,
            WRITE_NAME = 1,
            WRITE_TYPE = 2,
            WRITE_END_STRUCTURE = 4
        };

        #ifdef _MSC_VER
            #pragma warning(push)
            #pragma warning(disable: 4251)
        #endif
        std::unique_ptr<std::ostream> m_StreamPtr;
        std::ostream* m_Stream;
        std::stack<StateFlags> m_States;
        std::vector<char> m_Buffer;
        std::string m_Name;
        #ifdef _MSC_VER
            #pragma warning(pop)
        #endif
        static constexpr size_t BUFFER_SIZE = 16 * 1024;
    };
}
