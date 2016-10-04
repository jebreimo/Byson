#!/bin/bash

PROJECT_NAME=Byson
PROJECT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

RELEASE_DIR=release.$PROJECT_NAME
if [ ! -d $RELEASE_DIR ]
then
    mkdir $RELEASE_DIR
fi

cd $RELEASE_DIR
cmake -DCMAKE_BUILD_TYPE=Release $PROJECT_DIR
make install
cd ..

DEBUG_DIR=debug.$PROJECT_NAME
if [ ! -d $DEBUG_DIR ]
then
    mkdir $DEBUG_DIR
fi

cd $DEBUG_DIR
cmake -DCMAKE_BUILD_TYPE=Debug $PROJECT_DIR
make install
cd ..
