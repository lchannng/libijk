/*
 * File Name: singleton.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Fri 11 Mar 2016 12:19:47 AM CST
 */

#ifndef _SINGLETON_H
#define _SINGLETON_H

#include <stdlib.h>
#include <mutex>

namespace ijk {

template <typename T>
class singleton {
public:
    static T &instance() {
        std::call_once(once_, singleton::init);
        return *instance_;
    }

private:
    singleton() = default;
    ~singleton() = default;

    static void init() {
        instance_ = new T();
        ::atexit(singleton::destroy);
    }

    static void destroy() { delete instance_; }

private:
    static T *instance_;
    static std::once_flag once_;
};  // class Singleton

template <typename T>
T *singleton<T>::instance_ = NULL;

template <typename T>
std::once_flag singleton<T>::once_;

}  // namespace ijk
#endif
