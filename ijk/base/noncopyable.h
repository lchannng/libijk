/*
 * File Name: noncopyable.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2018/11/3 0:59:10
 */

#ifndef _NONCOPYABLE_H
#define _NONCOPYABLE_H

namespace ijk {
class noncopyable {
protected:
    noncopyable() = default;
    ~noncopyable() = default;
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};
}  // namespace ijk

#endif
