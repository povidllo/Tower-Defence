#!/bin/bash

set -e

echo "build start"

mkdir -p build
cd build

cmake ..
cmake --build .

TEMP_DIR=$(mktemp -d)

cd ..

tar -xzf "./rabota.tar.gz" -C "$TEMP_DIR"
rsync -av --ignore-existing --remove-source-files "$TEMP_DIR/" "./build/engine/"

cp -nv ./stub.jpg ./build/engine/

rm -rf "$TEMP_DIR"

echo "build finish"
