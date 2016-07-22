cmake_minimum_required(VERSION 3.5)
project(YubJson)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
elseif (WIN32)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS
        -DYSON_EXPORTS
        -DYSTRING_NO_AUTO_IMPORT)
endif (APPLE)

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
install(FILES ${PUBLIC_HEADERS}
    DESTINATION include/${PROJECT_NAME})
