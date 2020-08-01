#!/bin/bash
set -e

if [[ $1 == gcc || $1 == clang ]]; then
    compiler=$1
    shift
else
    compiler=clang
fi

proj_dir=$PWD
engine_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )/.." >/dev/null 2>&1 && pwd )"

# Use installed cmake if exists
PATH="$engine_dir/Tools/cmake-install/bin:$PATH"

cd "$proj_dir"
cmake --build out/build/${compiler} $1 -- -j $2
