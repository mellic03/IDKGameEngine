#!/bin/bash

# Build project
mkdir build output
cd build
cmake -G Ninja ..
ninja
cd ..

# Move output to output folder
cp build/IDKEngine output/IDKEngine
