#!/bin/bash
# Run unit tests for the assignment

mkdir -p build
cd build
cmake ..
make clean
make
cd ..
./build/assignment-autotest/test/assignment4/assignment-autotest
