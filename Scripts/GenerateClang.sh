#!/bin/bash
set -e

if command -v clang++-10 &> /dev/null; then
    echo "clang++-10 found"
    CC=clang-10
    CXX=clang++-10
elif ! command -v clang++ &> /dev/null; then
    echo "clang++ not found"
    clang_install=true
else
    clang_ver="$(clang++ --version | head -n1 | cut -d" " -f3)"
    clang_reqver=10
    if [ "$(printf '%s\n' "$clang_reqver" "$clang_ver" | sort -V | head -n1)" = "$clang_reqver" ]; then
        echo "clang++ $clang_ver found, suitable for required version $clang_reqver"
        CC=clang
        CXX=clang++
    else
        echo "clang++ $clang_ver found, but not compatible with required version $clang_reqver"
        clang_install=true
    fi
fi
if [ "$clang_install" = true ]; then
    sudo apt install -y clang-10 clang++-10 llvm-10-dev libstdc++-10-dev
    CC=clang-10
    CXX=clang++-10
fi

# Use installed cmake if exists
PATH="$PWD/Tools/cmake-install/bin:$PATH"

mkdir -p out/clang && cd out/clang
cmake ../.. -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX -D_CMAKE_TOOLCHAIN_PREFIX=llvm-
