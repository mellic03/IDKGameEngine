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
mkdir -p output/{include,lib}/IDKengine/

# Copy source code to output directory
cp -R src/. output/include/IDKengine/.

# Delete everything but header files
find ./output/include/IDKengine/ -name "*.cpp" -type f -delete
find ./output/include/IDKengine/ -name "*.txt" -type f -delete

# Copy library file to output directory
cp build/libIDKengine.a  output/lib/IDKengine/libIDKengine.a

