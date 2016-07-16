cmake_minimum_required(VERSION 3.5)
project(YubJson)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(HEADER_FILES
    YubJson/ContainerInfo.hpp
    YubJson/Reader.hpp
    YubJson/StringRef.hpp
    YubJson/Tokenizer.hpp
    YubJson/TokenizerPositionRestorer.hpp
    YubJson/TokenType.hpp
    YubJson/TokenTypeConversions.hpp
    YubJson/TokenTypeTable.hpp
    YubJson/ValueType.hpp
    YubJson/Writer.hpp
    YubJson/YubJsonDefinitions.hpp)

set(SOURCE_FILES
    YubJson/ContainerInfo.cpp
    YubJson/StringRef.cpp
    YubJson/Reader.cpp
    YubJson/Tokenizer.cpp
    YubJson/Writer.cpp)

add_library(${PROJECT_NAME}
    ${HEADER_FILES}
    ${SOURCE_FILES})
