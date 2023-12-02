#!/bin/bash

rm -R ./output
mkdir ./output ./output/diffuse ./output/specular

cp -R ./input/. ./output/.

./diffuseIBL 32 output/diffuse/
./specularIBL $1 128 output/specular/



