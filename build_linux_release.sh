#!/bin/sh
# File Name: build_linux_x64.sh
# Author: Stan.Lch
# mail: fn.stanc@gmail.com
# Created Time: 2018/10/29 11:10:11

mkdir -p ./_build/linux_release
cd ./_build/linux_release
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=./_install ../..
