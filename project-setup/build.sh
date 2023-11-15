#!/bin/bash

# Delete old files
rm -R ./output/assets

# Build project
mkdir ./build ./output ./output/assets ./output/IDKGE ./output/IDKGE/IDKtools
cd ./build
cmake -G Ninja ..
ninja
cd ..


cp ./build/game output/game
cp -R IDKGame/src/shaders output/.
cp -R IDKGame/assets ./output/.
cp -R ./IDKGameEngine/shaders ./output/IDKGE/.


rm -R ./output/assets/textures
mkdir ./output/assets/textures

cp ./build/texconvert ./output/IDKGE/IDKtools/texconvert
./build/texconvert ./IDKGame/assets/textures/diffuse  ./output/assets/textures/diffuse.texpak
./build/texconvert ./IDKGame/assets/textures/specular ./output/assets/textures/specular.texpak
./build/texconvert ./IDKGame/assets/textures/normal   ./output/assets/textures/normal.texpak
./build/texconvert ./IDKGame/assets/textures/reflection ./output/assets/textures/reflection.texpak
