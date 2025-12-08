#!/bin/sh

set -e

BUILD_DIR="build"
FORCE_RECONFIGURE="${1:-false}"

cd "$(dirname "$0")"

# 支持多种参数格式：true, --force, -f
if [ ! -f "${BUILD_DIR}/CMakeCache.txt" ] ||
  [ "$FORCE_RECONFIGURE" = "true" ] ||
  [ "$FORCE_RECONFIGURE" = "--force" ] ||
  [ "$FORCE_RECONFIGURE" = "-f" ]; then
  cmake -B ${BUILD_DIR} -S . -G Ninja \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_TOOLCHAIN_FILE="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
fi

cmake --build ${BUILD_DIR} -j 32
