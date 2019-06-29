#!/bin/sh
# File Name: build_win_x64.sh
# Author: Stan.Lch
# mail: fn.stanc@gmail.com
# Created Time: 2018/10/29 11:10:11

mkdir -p ./_build/win_x64_release
cd ./_build/win_x64_release
cmake -A x64 -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=E:/vcpkg/scripts/buildsystems/vcpkg.cmake ../..
