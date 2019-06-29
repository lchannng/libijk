/*
 * File Name: weak_callback.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Sat 18 Feb 2017 12:14:25 AM CST
 */

#ifndef _WEAK_CALLBACK_H
#define _WEAK_CALLBACK_H

#include <memory>

namespace ijk {

template <typename Guard, typename Function>
class WeakCallback {
private:
    std::weak_ptr<typename std::remove_reference<Guard>::type::element_type> g_;
    Function f_;

public:
    WeakCallback(Guard &&g, Function &&f)
        : g_(std::forward<Guard>(g)), f_(std::forward<Function>(f)) {}

    template <typename... Args>
    void operator()(Args &&... args) {
        if (g_.lock()) {
            f_(std::forward<Args>(args)...);
        }
    }
};

template <typename Guard, typename Function>
inline WeakCallback<Guard, Function> makeWeakCallback(Guard &&g, Function &&f) {
    return WeakCallback<Guard, Function>(std::forward<Guard>(g),
                                         std::forward<Function>(f));
}

}  // namespace ijk

#endif
