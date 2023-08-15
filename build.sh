#!/bin/bash

BUILD_FOLDER="./build"

if [ -d "$BUILD_FOLDER" ]; then
  rm -rf $BUILD_FOLDER
fi

mkdir $BUILD_FOLDER
cd $BUILD_FOLDER
cmake ..
make
cd ..