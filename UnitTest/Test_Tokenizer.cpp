//****************************************************************************
// Copyright © 2016, Jan Erik Breimo.
// Created 2014-04-24 by JEB
//
// This file is distributed under the Simplified BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "../Externals/Ytest/Ytest.hpp"
#include "../Byson/Tokenizer.hpp"

namespace
{
    using namespace Byson;

    void checkNextToken(Tokenizer& tokenizer, TokenType expectedTokenType)
    {
        Y_ASSERT(tokenizer.nextToken());
        Y_EQUAL(tokenizer.tokenType(), expectedTokenType);
    }

    template <typename T>
    void checkReadValue(Tokenizer& tokenizer, TokenType expectedTokenType, T expectedValue)
    {
        Y_ASSERT(tokenizer.nextToken());
        Y_EQUAL(tokenizer.tokenType(), expectedTokenType);
        T value;
        Y_ASSERT(tokenizer.read(value));
        Y_EQUAL(value, expectedValue);
    }

    void checkReadRawString(Tokenizer& tokenizer, const std::string& expectedValue)
    {
        std::string value(expectedValue.size(), '\0');
        Y_ASSERT(tokenizer.read(&value[0], value.size()));
        Y_EQUAL(value, expectedValue);
    }

    template <typename T, size_t N>
    void checkReadRawArray(Tokenizer& tokenizer, const T (&expectedValues)[N])
    {
        T values[N];
        Y_ASSERT(tokenizer.read(values, N));
        for (size_t i = 0; i < N; ++i)
            Y_EQUAL(values[i], expectedValues[i]);
    }

    void test_Object()
    {
        uint8_t data[] = {'{', 'i', 4, 'n', 'a', 'm', 'e',
                          'S', 'i', 3, 'J', 'a', 'n',
                          'i', 3, 'a', 'g', 'e', 'l', 0, 0, 0, 39, '}'};
        Tokenizer tokenizer(data, sizeof(data));
        Y_CALL(checkNextToken(tokenizer, TokenType::BEGIN_OBJECT));
        Y_CALL(checkReadValue<int8_t>(tokenizer, TokenType::INT8_VALUE, 4));
        Y_CALL(checkReadRawString(tokenizer, "name"));
        Y_CALL(checkNextToken(tokenizer, TokenType::STRING_VALUE));
        Y_CALL(checkReadValue<int8_t>(tokenizer, TokenType::INT8_VALUE, 3));
        Y_CALL(checkReadRawString(tokenizer, "Jan"));
        Y_CALL(checkReadValue<int8_t>(tokenizer, TokenType::INT8_VALUE, 3));
        Y_CALL(checkReadRawString(tokenizer, "age"));
        Y_CALL(checkReadValue<int32_t>(tokenizer, TokenType::INT32_VALUE, 39));
        Y_CALL(checkNextToken(tokenizer, TokenType::END_OBJECT));
    }

    void test_Array_optimized()
    {
        uint8_t data[] = {
                '[', '$', 'I', '#', 'i', 6, 0, 11, 0, 13, 0, 17, 0, 19, 0, 23,
                0, 29};
        Tokenizer tokenizer(data, sizeof(data));
        Y_CALL(checkNextToken(tokenizer, TokenType::BEGIN_ARRAY));
        Y_CALL(checkNextToken(tokenizer, TokenType::VALUE_TYPE));
        Y_CALL(checkNextToken(tokenizer, TokenType::INT16_VALUE));
        Y_CALL(checkNextToken(tokenizer, TokenType::COUNT));
        Y_CALL(checkReadValue<int8_t>(tokenizer, TokenType::INT8_VALUE, 6));
        int16_t expected[] = {11, 13, 17, 19, 23, 29};
        Y_CALL(checkReadRawArray(tokenizer, expected));
    }

    Y_TEST(test_Object, test_Array_optimized);

}
