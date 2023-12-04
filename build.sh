#!/bin/bash

# First make sure libidk has been built
# ----------------------------------------------------------------------------------------------
# cd libidk
# ./build.sh
# cd ../

# mkdir -p IDKGameEngine/external
# cp -R libidk/build/include IDKGameEngine/external/.
# cp -R libidk/build/lib     IDKGameEngine/external/.
# ----------------------------------------------------------------------------------------------


# Build IDK Engine
# ----------------------------------------------------------------------------------------------
mkdir -p build/CMake

cd build/CMake
cmake -G Ninja ../../
ninja -j 6
# ----------------------------------------------------------------------------------------------

cd ../
mkdir -p {include,lib,IDKGE/shipping}
mkdir -p include/{libidk,IDKengine}

cp CMake/libIDKengine.so* lib/
cp CMake/libIDKengine.so* IDKGE/shipping/
cp -R ../IDKGameEngine/src/* include/IDKengine/.
cp -R ../libidk/src/libidk   include/.

cp -R ../IDKGameEngine/shaders   ./IDKGE/.
cp -R ../IDKGameEngine/resources ./IDKGE/.

# Delete everything but header files
find ./include/IDKengine/ -name "*.cpp" -type f -delete
find ./include/IDKengine/ -name "*.txt" -type f -delete
find ./include/libidk/ -name "*.cpp" -type f -delete
find ./include/libidk/ -name "*.txt" -type f -delete
