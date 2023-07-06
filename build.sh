#!/bin/bash


# Build project
mkdir ./build ./output
cd ./build
cmake -G Ninja ..
ninja
cd ..


# Move output to output folder
cp ./build/IDKGame output/IDKGame
cp -R assets ./output/.
mkdir ./output/IDKtools
cp ./build/objconvert output/IDKtools/objconvert


# Copy IDK database files
cp -R IDKdb ./output/.


# Generate documentation
doxygen