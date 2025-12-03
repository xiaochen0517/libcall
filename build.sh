#!/bin/sh

set -e

BUILD_DIR="build"

cd "$(dirname "$0")"
# cmake -B build -S . -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
# cmake --build ./build

if [ ! -f "${BUILD_DIR}/CMakeCache.txt" ]; then
  cmake -B ${BUILD_DIR} -S . -G Ninja \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_TOOLCHAIN_FILE="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
fi

cmake --build ${BUILD_DIR} -j 32
