##****************************************************************************
## Copyright © 2016 Jan Erik Breimo. All rights reserved.
## Created by Jan Erik Breimo on 2016-07-17.
##
## This file is distributed under the Simplified BSD License.
## License text is included with the source distribution.
##****************************************************************************
cmake_minimum_required(VERSION 3.2)

set(PARENT_PROJECT YubJson)
project(${PARENT_PROJECT}Test)

set(SOURCE_FILES
    main.cpp
    Test_BufferWriter.cpp
    Test_StreamWriter.cpp
    Test_Tokenizer.cpp)

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
    ${PARENT_PROJECT}
    Ytest)

add_test(${PARENT_PROJECT} ${PROJECT_NAME})
