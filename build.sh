#!/bin/bash

# First make sure libidk has been built
# ----------------------------------------------------------------------------------------------
cd submodules/libidk/
./build.sh
cd ../../

mkdir -p external
cp -R submodules/libidk/output/* external/.
# ----------------------------------------------------------------------------------------------


# Build IDK Engine
# ----------------------------------------------------------------------------------------------
mkdir -p build

cd build
cmake -G Ninja ..
ninja -j 6
cd ..
# ----------------------------------------------------------------------------------------------


# Create output directory structure
mkdir -p output/{include,lib}/IDKEngine/

# Copy source code to output directory
cp -R src/. output/include/IDKEngine/.

# Delete everything but header files
find ./output/include/IDKEngine/ -name "*.cpp" -type f -delete
find ./output/include/IDKEngine/ -name "*.txt" -type f -delete

# Copy library file to output directory
cp build/libIDKengine.a  output/lib/IDKEngine/libIDKengine.a

