//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2014-04-24 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "../Externals/Ytest/Ytest.hpp"
#include "../YubJson/StreamWriter.hpp"
#include "Endian.hpp"

using namespace YubJson;

namespace
{
    void test_Object()
    {
        std::ostringstream ss(std::ios_base::out | std::ios_base::binary);
        StreamWriter writer(ss);
        writer.writeBeginObject();
        writer.writeValue("name", "JEB");
        writer.writeValue("age", 39);
        writer.writeEndObject();
        auto buffer = ss.str();

        uint8_t expected[] = {
                '{', 'i', 4, 'n', 'a', 'm', 'e', 'S', 'i', 3, 'J', 'E', 'B',
                'i', 3, 'a', 'g', 'e', 'l', 0, 0, 0, 39, '}'};
        Y_EQUAL(buffer.size(), sizeof(expected));
        Y_ASSERT(memcmp(buffer.data(), expected, buffer.size()) == 0);
    }

    void test_Array_optimized()
    {
        std::ostringstream ss(std::ios_base::out | std::ios_base::binary);
        StreamWriter writer(ss);
        writer.writeBeginArray(6, ValueType::Int16Value);
        int16_t values[4] = {13, 17, 19, 23};
        writer.writeRawValue(int16_t(11));
        writer.writeRawValues(values, sizeof(values) / sizeof(*values));
        writer.writeRawValue(int16_t(29));
        writer.writeEndArray();
        auto buffer = ss.str();

        std::string expected{
                '[', '$', 'I', '#', 'i', 6, 0, 11, 0, 13, 0, 17, 0, 19, 0, 23,
                0, 29};
        Y_EQUAL_RANGES(buffer, expected);
    }

    template <typename T>
    void checkWriteShortest(int64_t value, char expectedMarker)
    {
        std::ostringstream ss(std::ios_base::out | std::ios_base::binary);
        StreamWriter writer(ss);
        writer.writeShortestValue(value);
        auto buffer = ss.str();
        Y_EQUAL(buffer.size(), sizeof(T) + 1);
//        auto buffer = static_cast<const uint8_t*>(writer.buffer());
        Y_EQUAL(buffer[0], expectedMarker);
        auto expectedValue = static_cast<T>(value);
        Y_EQUAL(*reinterpret_cast<const T*>(&buffer[1]),
                JEBBase::Bits::bigEndian(expectedValue));
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

    Y_TEST(test_Object, test_Array_optimized, test_WriteShortest);

}
