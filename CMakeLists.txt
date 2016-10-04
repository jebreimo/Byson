cmake_minimum_required(VERSION 3.5)
project(Byson)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
elseif (WIN32)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS
        -DBYSON_EXPORTS)
endif (APPLE)

set(PUBLIC_HEADER_FILES
    Byson/ContainerInfo.hpp
    Byson/Endian.hpp
    Byson/BufferReader.hpp
    Byson/BufferWriter.hpp
    Byson/StreamWriter.hpp
    Byson/StringRef.hpp
    Byson/Tokenizer.hpp
    Byson/TokenizerPositionRestorer.hpp
    Byson/TokenType.hpp
    Byson/TokenTypeConversions.hpp
    Byson/ValueType.hpp
    Byson/BysonDefinitions.hpp)

set(PRIVATE_HEADER_FILES
    Byson/TokenTypeTable.hpp)

set(SOURCE_FILES
    Byson/ContainerInfo.cpp
    Byson/BufferReader.cpp
    Byson/BufferWriter.cpp
    Byson/StringRef.cpp
    Byson/Tokenizer.cpp
    Byson/StreamWriter.cpp)

add_library(${PROJECT_NAME}
    ${PUBLIC_HEADER_FILES}
    ${PRIVATE_HEADER_FILES}
    ${SOURCE_FILES})

add_subdirectory(Externals)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    MACOSX_RPATH ON
    DEBUG_OUTPUT_NAME "${PROJECT_NAME}.debug")

if (HAS_YTEST)
    set(CTEST_OUTPUT_ON_FAILURE 1)
    add_subdirectory(UnitTest)
endif (HAS_YTEST)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${PUBLIC_HEADER_FILES}
    DESTINATION include/${PROJECT_NAME})
