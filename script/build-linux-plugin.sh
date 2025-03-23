#!/bin/bash
set -e

cmake -Bbuild -S. \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_TOOLCHAIN_FILE=cmake/linux-toolchain.cmake \
  -DBUILD_DESKTOP=ON

cmake --build build
