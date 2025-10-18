#!/bin/bash
set -e


export CXX=clang++
export CC=clang
mkdir -p build
cmake -S . -B build -DCMAKE_CXX_FLAGS="-stdlib=libc++"
cmake --build build

mkdir -p sandbox

cd sandbox
../build/cbutler run
