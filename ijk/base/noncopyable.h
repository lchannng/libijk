/*
 * File Name: noncopyable.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2018/11/3 0:59:10
 */

#ifndef _NONCOPYABLE_H
#define _NONCOPYABLE_H

namespace ijk {
class Noncopyable {
protected:
    Noncopyable() = default;
    ~Noncopyable() = default;
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};
}  // namespace ijk

#endif
