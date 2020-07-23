#!/bin/bash
set -e
work_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

if command -v g++-10 &> /dev/null; then
    echo "g++-10 found"
    CC=gcc-10
    CXX=g++-10
elif ! command -v g++ &> /dev/null; then
    echo "g++ not found"
    gcc_install=true
else
    gcc_ver="$(g++ --version | head -n1 | cut -d" " -f4)"
    gcc_reqver=10
    if [ "$(printf '%s\n' "$gcc_reqver" "$gcc_ver" | sort -V | head -n1)" = "$gcc_reqver" ]; then
        echo "g++ $gcc_ver found, suitable for required version $gcc_reqver"
        CC=gcc
        CXX=g++
    else
        echo "g++ $gcc_ver found, but not compatible with required version $gcc_reqver"
        gcc_install=true
    fi
fi
if [ "$gcc_install" = true ]; then
    sudo apt install -y gcc-10 g++-10 libstdc++-10-dev
    CC=gcc-10
    CXX=g++-10
fi

cd $work_dir
mkdir -p out && cd out

PATH=$work_dir/Tools/cmake-install/bin:$PATH
cmake .. -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX
