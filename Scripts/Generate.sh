#!/bin/bash
set -e

gcc_reqver=10
gcc_cxx=g++

clang_reqver=10
clang_cxx=clang++
clang_ext_pkg=llvm-${clang_reqver}-dev
clang_cmake_arg="-D_CMAKE_TOOLCHAIN_PREFIX=llvm-"

if [[ $1 == gcc || $1 == clang ]]; then
    compiler=$1
    shift
else
    compiler=clang
fi

reqver=${compiler}_reqver
compiler_cxx=${compiler}_cxx
ext_pkg=${compiler}_ext_pkg
cmake_arg=${compiler}_cmake_arg

should_install=false

if command -v ${!compiler_cxx}-${!reqver} &> /dev/null; then
    echo "${!compiler_cxx}-${!reqver} found"
    CC=${compiler}-${!reqver}
    CXX=${!compiler_cxx}-${!reqver}
elif ! command -v ${!compiler_cxx} &> /dev/null; then
    echo "${!compiler_cxx} not found"
    should_install=true
else
    found_ver="$(${!compiler_cxx} --version | head -n1 | cut -d" " -f3)"
    if [ "$(printf '%s\n' "${!reqver}" "$found_ver" | sort -V | head -n1)" = "${!reqver}" ]; then
        echo "${!compiler_cxx} $found_ver found, suitable for required version ${!reqver}"
        CC=${compiler}
        CXX=${!compiler_cxx}
    else
        echo "${!compiler_cxx} $found_ver found, but not compatible with required version ${!reqver}"
        should_install=true
    fi
fi
if [[ "$should_install" == true ]]; then
    sudo apt install -y ${compiler}-${!reqver} ${!compiler_cxx}-${!reqver} libstdc++-${!reqver}-dev ${!ext_pkg}
    CC=${compiler}-${!reqver}
    CXX=${!compiler_cxx}-${!reqver}
fi

# Use installed cmake if exists
PATH="$PWD/Tools/cmake-install/bin:$PATH"

mkdir -p out/${compiler} && cd out/${compiler}
cmake ../.. -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX ${!cmake_arg} "$@"
