#!/bin/bash

set -e

g++ -std=c++17 -Wall -Wextra \
    chell.cpp \
    src/executor.cpp \
    src/builtins.cpp \
    src/utils.cpp \
    -o shell

echo "Build successful."