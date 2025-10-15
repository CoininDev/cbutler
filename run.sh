#!/bin/bash
set -e

mkdir -p build
cmake -S . -B build
cmake --build build

mkdir -p sandbox

cd sandbox
../build/cbutler 
