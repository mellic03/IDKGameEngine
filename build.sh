#!/bin/bash

# mkdir -p ./external/{include,lib}


# Build libidk
# ----------------------------------------------------------------------------------------------
cd ./libidk
./build.sh
cd ../

# Copy libidk header files
cp -R ./libidk/build/include/*   ./external/include/.
cp -R ./libidk/build/lib/*       ./external/lib/.
# ----------------------------------------------------------------------------------------------



# Build IDK Engine
# ----------------------------------------------------------------------------------------------
mkdir -p build/{.CMake,output}

cd build/.CMake
cmake -G Ninja ../../
ninja -j 12
# ----------------------------------------------------------------------------------------------




# Copy headers
# ----------------------------------------------------------------------------------------------
cd ../

# Copy all source files
mkdir -p ./include/{libidk,IDKengine}
cp -R ../IDKGameEngine/src/* ./include/IDKengine/.

# Copy libidk header files
cp -R ../libidk/build/include/*   ./include/.
cp -R ../libidk/build/lib/*       ./lib/.


# Delete everything except for headers
find ./include/IDKengine/ -name "*.cpp" -type f -delete
find ./include/IDKengine/ -name "*.txt" -type f -delete
find ./include/libidk/    -name "*.cpp" -type f -delete
find ./include/libidk/    -name "*.txt" -type f -delete
# ----------------------------------------------------------------------------------------------


# Copy IDKGE stuff to output folder
# ----------------------------------------------------------------------------------------------
cp -R ../IDKGameEngine/IDKGE   ./output/.
cp ./lib/libIDKengine.*      ./output/IDKGE/runtime/.

# Copy scripts
cp -R ../IDKGameEngine/scripts ./.

# Copy libIDKengine into GameTemplate
cp -R ./include ./scripts/GameTemplate/external/.
cp -R ./lib     ./scripts/GameTemplate/external/.
# ----------------------------------------------------------------------------------------------



# Copy needed files to other projects
# ----------------------------------------------------------------------------------------------
# cp -R ./include ../../IDKtools/src/Module-Create/EditorUI2/external/.
# cp -R ./lib     ../../IDKtools/src/Module-Create/EditorUI2/external/.


# cp -R ./include ../../IDKGame/external/.
# cp -R ./lib     ../../IDKGame/external/.


# cp -R ./include ../../IDKGameLoader/external/.
# cp -R ./lib     ../../IDKGameLoader/external/.
# cp -R ../IDKGameEngine/IDKGE  ../../IDKGameLoader/external/.
# cp    ./lib/libIDKengine.so   ../../IDKGameLoader/external/IDKGE/runtime/.
# ----------------------------------------------------------------------------------------------


# cd ../
# mkdir -p {include,lib,IDKGE/runtime}
# mkdir -p include/{libidk,IDKengine}

# cp CMake/libIDKengine.so* lib/
# cp CMake/libIDKengine.so* IDKGE/runtime/
# cp -R ../IDKGameEngine/src/* include/IDKengine/.
# cp -R ../libidk/libidk   include/.

# cp -R ../IDKGameEngine/shaders   ./IDKGE/.
# cp -R ../IDKGameEngine/resources ./IDKGE/.

# # Delete everything but header files
# find ./include/IDKengine/ -name "*.cpp" -type f -delete
# find ./include/IDKengine/ -name "*.txt" -type f -delete
# find ./include/libidk/ -name "*.cpp" -type f -delete
# find ./include/libidk/ -name "*.txt" -type f -delete

# cd ..
# cp -R ./build/include ../IDKGame/external/.
# cp -R ./build/lib     ../IDKGame/external/.
# cp -R ./build/IDKGE   ../IDKGame/output/.
