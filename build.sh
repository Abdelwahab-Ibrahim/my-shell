#!/bin/bash

set -e

g++ -std=c++17 -Wall -Wextra \
    src/chell.cpp \
    src/executor.cpp \
    -o shell

echo "Build successful."