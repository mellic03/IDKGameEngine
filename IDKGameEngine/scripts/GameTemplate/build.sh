#!/bin/bash

mkdir -p ./build/CMake

cd ./build/CMake
cmake -G Ninja ../../
ninja -j 12

# cd ../
# cp ./CMake/*.so* ./.

