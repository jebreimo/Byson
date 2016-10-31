//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2014-04-24 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "../Byson/BufferWriter.hpp"
#include "../Externals/Ytest/Ytest.hpp"

using namespace Byson;

namespace
{
    void test_Object()
    {
        BufferWriter writer;
        writer.writeBeginObject();
        writer.writeValue("name", "JEB");
        writer.writeValue("age", 39);
        writer.writeEndObject();

        uint8_t expected[] = {
                '{', 'i', 4, 'n', 'a', 'm', 'e', 'S', 'i', 3, 'J', 'E', 'B',
                'i', 3, 'a', 'g', 'e', 'l', 0, 0, 0, 39, '}'};
        Y_EQUAL(writer.size(), sizeof(expected));
        Y_ASSERT(memcmp(writer.buffer(), expected, writer.size()) == 0);
    }

    void test_Array_optimized()
    {
        BufferWriter writer;
        writer.writeBeginArray(6, ValueType::INT16_VALUE);
        int16_t values[4] = {13, 17, 19, 23};
        writer.writeRawValue(int16_t(11));
        writer.writeRawValues(values, sizeof(values) / sizeof(*values));
        writer.writeRawValue(int16_t(29));
        writer.writeEndArray();

        uint8_t expected[] = {
                '[', '$', 'I', '#', 'i', 6, 0, 11, 0, 13, 0, 17, 0, 19, 0, 23,
                0, 29};
        Y_EQUAL(writer.size(), sizeof(expected));
        Y_ASSERT(memcmp(writer.buffer(), expected, writer.size()) == 0);
    }

    void test_Optimized_array_of_arrays()
    {
        BufferWriter writer;
        writer.writeBeginArray(2, ValueType::ARRAY_VALUE);
        writer.writeBeginArray(2, ValueType::INT8_VALUE);
        writer.writeValue(int8_t(1));
        writer.writeValue(int8_t(2));
        writer.writeEndArray();
        writer.writeBeginArray(2, ValueType::INT8_VALUE);
        writer.writeValue(int8_t(3));
        writer.writeValue(int8_t(4));
        writer.writeEndArray();
        writer.writeEndArray();

        std::vector<uint8_t> expected {
                '[', '$', '[', '#', 'i', 2, 'i', 1, 'i', 2, ']', 'i', 3, 'i',
                4, ']'};
        Y_EQUAL_RANGES(writer.vector(), expected);
    }

    void test_Array_of_optimized_arrays()
    {
        BufferWriter writer;
        writer.writeBeginArray();
        writer.writeBeginArray(2, ValueType::INT8_VALUE);
        writer.writeValue(int8_t(1));
        writer.writeValue(int8_t(2));
        writer.writeEndArray();
        writer.writeBeginArray(2, ValueType::INT8_VALUE);
        writer.writeValue(int8_t(3));
        writer.writeValue(int8_t(4));
        writer.writeEndArray();
        writer.writeEndArray();

        std::vector<uint8_t> expected{
                '[', '[', '$', 'i', '#', 'i', 2, 1, 2, '[', '$', 'i',
                '#', 'i', 2, 3, 4, ']'};
        Y_EQUAL_RANGES(writer.vector(), expected);
    }

    template <typename T>
    void checkWriteShortest(int64_t value, char expectedMarker)
    {
        BufferWriter writer;
        writer.writeShortestValue(value);
        Y_EQUAL(writer.size(), sizeof(T) + 1);
        auto buffer = static_cast<const uint8_t*>(writer.buffer());
        Y_EQUAL(buffer[0], expectedMarker);
        auto expectedValue = static_cast<T>(value);
        Y_EQUAL(*reinterpret_cast<const T*>(&buffer[1]),
                bigEndian(expectedValue));
    }

    void test_WriteShortest()
    {
        Y_CALL(checkWriteShortest<int8_t>(0, 'i'));
        Y_CALL(checkWriteShortest<int8_t>(127, 'i'));
        Y_CALL(checkWriteShortest<uint8_t>(128, 'U'));
        Y_CALL(checkWriteShortest<uint8_t>(255, 'U'));
        Y_CALL(checkWriteShortest<int16_t>(256, 'I'));
        Y_CALL(checkWriteShortest<int16_t>(32767, 'I'));
        Y_CALL(checkWriteShortest<int32_t>(32768, 'l'));
        Y_CALL(checkWriteShortest<int32_t>(2147483647ll, 'l'));
        Y_CALL(checkWriteShortest<int64_t>(2147483648ll, 'L'));
        Y_CALL(checkWriteShortest<int8_t>(-1, 'i'));
        Y_CALL(checkWriteShortest<int8_t>(-128, 'i'));
        Y_CALL(checkWriteShortest<int16_t>(-129, 'I'));
        Y_CALL(checkWriteShortest<int16_t>(-32768, 'I'));
        Y_CALL(checkWriteShortest<int32_t>(-32769, 'l'));
        Y_CALL(checkWriteShortest<int32_t>(-2147483648ll, 'l'));
        Y_CALL(checkWriteShortest<int64_t>(-2147483649ll, 'L'));
    }

    Y_TEST(test_Object,
           test_Array_optimized,
           test_WriteShortest,
           test_Optimized_array_of_arrays,
           test_Array_of_optimized_arrays);
}
