#!/bin/bash
set -e

[[ $1 ]] && ver=$1 || ver=3.18.1

if [[ ! -f cmake-install/bin/cmake ]]; then
    mkdir -p Downloads
    cd Downloads

    wget -nv -nc https://github.com/Kitware/CMake/releases/download/v$ver/cmake-$ver-Linux-x86_64.tar.gz
    tar -xf cmake-$ver-Linux-x86_64.tar.gz
    mv cmake-$ver-Linux-x86_64 ../cmake-install
fi
