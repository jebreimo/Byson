//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2014-04-24 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cstdint>
#include <vector>
#include "YubJsonDefinitions.hpp"
#include "ValueType.hpp"

namespace YubJson
{
    class UBJSON_API Writer
    {
    public:
        Writer();

        ~Writer();

        void clear();

        const void* buffer() const;

        size_t size() const;

        void beginObject();

        void beginOptimizedObject(size_t count,
                                  ValueType valueType = ValueType::UndefinedValue);

        void endObject();

        void beginArray();

        void beginOptimizedArray(size_t count,
                                 ValueType valueType = ValueType::UndefinedValue);

        void endArray();

        void key(const char* key, size_t length);

        void key(const std::string& key);

        void writeNull();

        void writeNoOp();

        void write(bool value);

        void write(int8_t value);

        void write(uint8_t value);

        void write(int16_t value);

        void write(uint16_t value);

        void write(int32_t value);

        void write(uint32_t value);

        void write(int64_t value);

        void write(uint64_t value);

        void write(float value);

        void write(double value);

        void write(char value);

        void write(const char* value, size_t length);

        void write(const std::string& value);

        template <size_t N>
        void write(const char (& value)[N])
        {
            write(value, value[N - 1] ? N : N - 1);
        }

        void writeShortest(int64_t value);

        void writeRaw(int8_t value);

        void writeRaw(uint8_t value);

        void writeRaw(int16_t value);

        void writeRaw(int32_t value);

        void writeRaw(int64_t value);

        void writeRaw(float value);

        void writeRaw(double value);

        void writeRawChar(char value);

        void writeRaw(const char* value, size_t length);

        void writeRaw(const std::string& value);

        template <typename T>
        void writeRaw(const T* values, size_t count)
        {
            auto bufSize = m_Buffer.size();
            m_Buffer.resize(bufSize + count * sizeof(T));
            auto alias = reinterpret_cast<const uint8_t*>(values);
            auto dst = begin(m_Buffer) + bufSize;
            auto index = sizeof(T);
            for (auto i = 0; i < count; ++i)
            {
                for (auto j = 0; j < sizeof(T); ++j)
                    *dst++ = alias[--index];
                index += 2 * sizeof(T);
            }
        }

    private:
        void writeValueType(ValueType valueType);

        void writeCount(size_t count);

        #ifdef _MSC_VER
            #pragma warning(push)
            #pragma warning(disable: 4251)
        #endif
        std::vector<uint8_t> m_Buffer;
        #ifdef _MSC_VER
            #pragma warning(pop)
        #endif
    };
}
