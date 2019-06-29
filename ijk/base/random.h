/*
 * File Name: random.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2018/4/22 20:39:53
 */

#ifndef _IJK_RANDOM_H
#define _IJK_RANDOM_H

#include <random>
#include <string>

namespace ijk {

inline std::string randomString(size_t len) {
    static const std::string::value_type allowed_chars[]{
        "123456789ABCDFGHJKLMNPQRSTVWXZabcdfghjklmnpqrstvwxz"};
    static thread_local std::default_random_engine rg(std::random_device{}());
    static thread_local std::uniform_int_distribution<int> dist(
        0, sizeof(allowed_chars) - 2);

    std::string res(len, '\0');
    for (std::string::value_type& c : res) {
        c = allowed_chars[dist(rg)];
    }
    return res;
}

}  // namespace ijk

#endif
