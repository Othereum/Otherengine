#!/bin/bash
cd "$( dirname "${BASH_SOURCE[0]}" )/.."
rm -rf docs
doxygen
