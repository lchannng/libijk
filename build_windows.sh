#!/bin/sh
# File Name: build_win_x64.sh
# Author: Stan.Lch
# mail: fn.stanc@gmail.com
# Created Time: 2018/10/29 11:10:11

install_dir="`pwd`/_install"

mkdir -p ./_build/windows
cd ./_build/windows
cmake -Wno-dev -A x64 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=E:/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_INSTALL_PREFIX=${install_dir} ../..
