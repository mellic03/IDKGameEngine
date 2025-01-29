#!/bin/bash
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

cp -R $SCRIPT_DIR/template/. ./$1/
cd ./$1/src

sed -i -e "s/TemplateGameName/$1/g" ./game.hpp
sed -i -e "s/TemplateGameName/$1/g" ./main.cpp
sed -i -e "s/TemplateGameName/$1/g" ./export.cpp


