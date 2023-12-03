#!/bin/bash

# First make sure libidk has been built
# ----------------------------------------------------------------------------------------------
cd libidk
./build.sh
cd ../

mkdir -p IDKGameEngine/external
cp -R libidk/build/include IDKGameEngine/external/.
cp -R libidk/build/lib     IDKGameEngine/external/.
# ----------------------------------------------------------------------------------------------


# Build IDK Engine
# ----------------------------------------------------------------------------------------------
mkdir -p build/CMake

cd build/CMake
cmake -G Ninja ../../IDKGameEngine
ninja -j 6
# ----------------------------------------------------------------------------------------------

cd ../
mkdir -p {include,lib}
mkdir -p include/IDKengine

cp CMake/libIDKengine.so lib/.
cp -R ../IDKGameEngine/src/* include/IDKengine/.

# Delete everything but header files
find ./include/IDKengine/ -name "*.cpp" -type f -delete
find ./include/IDKengine/ -name "*.txt" -type f -delete

cd ../
cp -R libidk/build/include/* build/include/.
cp -R libidk/build/lib/* build/lib/.
