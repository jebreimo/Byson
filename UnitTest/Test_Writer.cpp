//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2014-04-24 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "../Externals/Ytest/Ytest.hpp"
#include "../YubJson/Writer.hpp"
#include "Endian.hpp"

using namespace YubJson;

namespace
{
    void test_Object()
    {
        Writer writer;
        writer.beginObject();
        writer.key("name");
        writer.write("JEB");
        writer.key("age");
        writer.write(39);
        writer.endObject();

        uint8_t expected[] = {
                '{', 'i', 4, 'n', 'a', 'm', 'e', 'S', 'i', 3, 'J', 'E', 'B',
                'i', 3, 'a', 'g', 'e', 'l', 0, 0, 0, 39, '}'};
        Y_EQUAL(writer.size(), sizeof(expected));
        Y_ASSERT(memcmp(writer.buffer(), expected, writer.size()) == 0);
    }

    void test_Array_optimized()
    {
        Writer writer;
        writer.beginOptimizedArray(6, ValueType::Int16Value);
        int16_t values[4] = {13, 17, 19, 23};
        writer.writeRaw(int16_t(11));
        writer.writeRaw(values, sizeof(values) / sizeof(*values));
        writer.writeRaw(int16_t(29));

        uint8_t expected[] = {
                '[', '$', 'I', '#', 'i', 6, 0, 11, 0, 13, 0, 17, 0, 19, 0, 23,
                0, 29};
        Y_EQUAL(writer.size(), sizeof(expected));
        Y_ASSERT(memcmp(writer.buffer(), expected, writer.size()) == 0);
    }

    template <typename T>
    void
    checkWriteShortest(Writer& writer, int64_t value, char expectedMarker)
    {
        writer.clear();
        writer.writeShortest(value);
        Y_EQUAL(writer.size(), sizeof(T) + 1);
        auto buffer = static_cast<const uint8_t*>(writer.buffer());
        Y_EQUAL(buffer[0], expectedMarker);
        auto expectedValue = static_cast<T>(value);
        Y_EQUAL(*reinterpret_cast<const T*>(&buffer[1]),
                JEBBase::Bits::bigEndian(expectedValue));
    }

    void test_WriteShortest()
    {
        Writer writer;
        Y_CALL(checkWriteShortest<int8_t>(writer, 0, 'i'));
        Y_CALL(checkWriteShortest<int8_t>(writer, 127, 'i'));
        Y_CALL(checkWriteShortest<uint8_t>(writer, 128, 'U'));
        Y_CALL(checkWriteShortest<uint8_t>(writer, 255, 'U'));
        Y_CALL(checkWriteShortest<int16_t>(writer, 256, 'I'));
        Y_CALL(checkWriteShortest<int16_t>(writer, 32767, 'I'));
        Y_CALL(checkWriteShortest<int32_t>(writer, 32768, 'l'));
        Y_CALL(checkWriteShortest<int32_t>(writer, 2147483647ll, 'l'));
        Y_CALL(checkWriteShortest<int64_t>(writer, 2147483648ll, 'L'));
        Y_CALL(checkWriteShortest<int8_t>(writer, -1, 'i'));
        Y_CALL(checkWriteShortest<int8_t>(writer, -128, 'i'));
        Y_CALL(checkWriteShortest<int16_t>(writer, -129, 'I'));
        Y_CALL(checkWriteShortest<int16_t>(writer, -32768, 'I'));
        Y_CALL(checkWriteShortest<int32_t>(writer, -32769, 'l'));
        Y_CALL(checkWriteShortest<int32_t>(writer, -2147483648ll, 'l'));
        Y_CALL(checkWriteShortest<int64_t>(writer, -2147483649ll, 'L'));
    }

    Y_TEST(test_Object, test_Array_optimized, test_WriteShortest);

}
