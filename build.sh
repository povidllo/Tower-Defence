#!/bin/bash

set -e

echo "build start"

mkdir -p build
cd build

cmake ..
cmake --build .

echo "build finish"
