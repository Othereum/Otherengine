#!/bin/bash
set -e
work_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

if command -v gcc-10 &> /dev/null; then
    echo "gcc-10 found"
    CC=gcc-10
    CXX=g++-10
elif ! command -v gcc &> /dev/null; then
    echo "gcc not found"
    gcc_install=true
else
    gcc_ver="$(gcc --version | head -n1 | cut -d" " -f4)"
    gcc_reqver=10
    if [ "$(printf '%s\n' "$gcc_reqver" "$gcc_ver" | sort -V | head -n1)" = "$gcc_reqver" ]; then
        echo "gcc $gcc_ver found, suitable for required version $gcc_reqver"
        CC=gcc
        CXX=g++
    else
        echo "gcc $gcc_ver found, but not compatible with required version $gcc_reqver"
        gcc_install=true
    fi
fi
if [ "$gcc_install" = true ]; then
    sudo apt install -y gcc-10 g++-10
    CC=gcc-10
    CXX=g++-10
fi

cd $work_dir
mkdir -p out && cd out
cmake ..
