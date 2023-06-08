#!/bin/bash
find './src/' \( -name "*.cpp" -o -name "*.h" \) -not -path './src/include/*' | xargs wc -l
find './assets/shaders/' \( -name "*.vs" -o -name "*.fs" \) | xargs wc -l