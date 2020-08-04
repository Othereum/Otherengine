#!/bin/bash
set -e

if [[ ! -d Source/ThirdParty/fmod ]]; then
    mkdir -p Downloads
    cd Downloads

    if [[ ! -f fmodstudioapi$3linux.tar.gz ]]; then
        TOKEN=$(curl -X POST -u $1:$2 https://www.fmod.com/api-login | jq -r '.token')
        URL=$(curl -H 'Accept: application/json' -H "Authorization: Bearer ${TOKEN}" https://www.fmod.com/api-get-download-link\?path\=files/fmodstudio/api/Linux/\&filename\=fmodstudioapi$3linux.tar.gz\&user\=$1 | jq -r '.url')
        wget -O fmodstudioapi$3linux.tar.gz $URL
    fi

    tar -xf fmodstudioapi$3linux.tar.gz
    mv fmodstudioapi$3linux ../Source/ThirdParty/fmod
fi
