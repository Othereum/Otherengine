#!/bin/bash
set -e

[[ $3 ]] && ver=$3 || ver=20010

if [[ ! -d Source/ThirdParty/fmod ]]; then
    mkdir -p Downloads
    cd Downloads

    if [[ ! -f fmodstudioapi${ver}linux.tar.gz ]]; then
        TOKEN=$(curl -X POST -u $1:$2 https://www.fmod.com/api-login | jq -r '.token')
        URL=$(curl -H 'Accept: application/json' -H "Authorization: Bearer ${TOKEN}" https://www.fmod.com/api-get-download-link\?path\=files/fmodstudio/api/Linux/\&filename\=fmodstudioapi${ver}linux.tar.gz\&user\=$1 | jq -r '.url')
        wget -nv -O fmodstudioapi${ver}linux.tar.gz $URL
    fi

    tar -xf fmodstudioapi${ver}linux.tar.gz
    mv fmodstudioapi${ver}linux ../Source/ThirdParty/fmod
fi

