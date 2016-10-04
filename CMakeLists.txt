cmake_minimum_required(VERSION 3.5)
project(YubJson)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
elseif (WIN32)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS
        -DYSON_EXPORTS
        -DYSTRING_NO_AUTO_IMPORT)
endif (APPLE)

set(PUBLIC_HEADER_FILES
    YubJson/ContainerInfo.hpp
    YubJson/Endian.hpp
    YubJson/BufferReader.hpp
    YubJson/BufferWriter.hpp
    YubJson/StreamWriter.hpp
    YubJson/StringRef.hpp
    YubJson/Tokenizer.hpp
    YubJson/TokenizerPositionRestorer.hpp
    YubJson/TokenType.hpp
    YubJson/TokenTypeConversions.hpp
    YubJson/ValueType.hpp
    YubJson/YubJsonDefinitions.hpp)

set(PRIVATE_HEADER_FILES
    YubJson/TokenTypeTable.hpp)

set(SOURCE_FILES
    YubJson/ContainerInfo.cpp
    YubJson/BufferReader.cpp
    YubJson/BufferWriter.cpp
    YubJson/StringRef.cpp
    YubJson/Tokenizer.cpp
    YubJson/StreamWriter.cpp)

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
