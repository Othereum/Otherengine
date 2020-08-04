#!/bin/bash
set -e

if [[ ! -f cmake-install/bin/cmake ]]; then
    mkdir -p Downloads
    cd Downloads

    wget -nc https://github.com/Kitware/CMake/releases/download/v$1/cmake-$1-Linux-x86_64.tar.gz
    tar -xf cmake-$1-Linux-x86_64.tar.gz
    mv cmake-$1-Linux-x86_64 ../cmake-install
fi
