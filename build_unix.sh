#!/bin/sh
# File Name: build_linux_x64.sh
# Author: Stan.Lch
# mail: fn.stanc@gmail.com
# Created Time: 2018/10/29 11:10:11

install_dir="`pwd`/_install"

mkdir -p ./_build/unix
cd ./_build/unix
cmake -A x64 -DCMAKE_BUILD_TYPE=Debug \
    -DBUILD_TESTS=ON \
    -DBUILD_SAMPLES=ON \
    -DCMAKE_INSTALL_PREFIX=${install_dir} ../..
