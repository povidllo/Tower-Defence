#!/bin/bash

set -e

#rm -rf build CMakeUserPresets.json

conan install . --build=missing

cmake --preset conan-release

cmake --build --preset conan-release

TEMP_DIR=$(mktemp -d)

tar -xzf "./rabota.tar.gz" -C "$TEMP_DIR"
rsync -av --ignore-existing --remove-source-files "$TEMP_DIR/" "./build/Release/engine/"

cp -nv ./stub.jpg ./build/Release/engine/

rm -rf "$TEMP_DIR"

echo "build finish"
