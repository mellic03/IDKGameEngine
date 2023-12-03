#!/bin/bash

# First make sure libidk has been built
# ----------------------------------------------------------------------------------------------
cd IDKGameEngine/submodules/libidk/
./build.sh
cd ../../

mkdir -p external
cp -R submodules/libidk/build/include external/.
cp -R submodules/libidk/build/lib     external/.
# ----------------------------------------------------------------------------------------------


# Build IDK Engine
# ----------------------------------------------------------------------------------------------
cd ../
mkdir -p build/CMake

cd build/CMake
cmake -G Ninja ../../IDKGameEngine
ninja -j 6
# ----------------------------------------------------------------------------------------------

cd ../
mkdir -p {include,lib}/
mkdir -p include/IDKengine

cp CMake/libIDKengine.so lib/.
cp -R ../IDKGameEngine/src/* include/IDKengine/.

# Delete everything but header files
find ./include/IDKengine/ -name "*.cpp" -type f -delete
find ./include/IDKengine/ -name "*.txt" -type f -delete

cd ../
cp -R IDKGameEngine/submodules/libidk/build/include/* build/include/.
cp -R IDKGameEngine/submodules/libidk/build/lib/* build/lib/.
