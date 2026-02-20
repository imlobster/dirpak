#!/bin/sh

mkdir -p build
cd build || exit 1
cmake ..
printf "\nbuilding...\n\n"
cmake --build $@ .
cd ..
