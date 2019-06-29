#!/bin/sh

find ijk tests \
    \( -name '*.cc' -o -name '*.cpp' -o -name '*.hpp' -o -name '*.ipp' -o -name '*.c' -o -name '*.h' \) \
    -not \( \
        -path 'ijk/base/details/fmt/*' \
        -o -path 'ijk/base/details/gsl/*' \
        -o -path 'ijk/base/details/nonstd/*' \
        -o -path 'ijk/base/details/termcolor*' \
        -o -path 'ijk/io/asio/*' \
        -o -path 'samples/shadowsocks-ijk/common/json11.*' \
        -o -path 'extras/redis/3rd/*' \
    \) \
    -exec clang-format -i {} ';'
    #-exec echo {} ';'
