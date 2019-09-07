#!/bin/sh

find ijk tests \
    \( -name '*.cc' -o -name '*.cpp' -o -name '*.hpp' -o -name '*.ipp' -o -name '*.c' -o -name '*.h' \) \
    -not \( \
        -path 'ijk/base/details/*' \
        -o -path 'ijk/future/*' \
    \) \
    -exec clang-format -i {} ';'
    #-exec echo {} ';'
