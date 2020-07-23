#!/bin/bash
set -e

if [[ $1 == gcc || $1 == clang ]]; then
    compiler=$1
    shift
else
    compiler=clang
fi

# Use installed cmake if exists
PATH="$PWD/Tools/cmake-install/bin:$PATH"

cmake --build out/${compiler} $1 -- -j $2
