/*
 * File Name: scope_guard.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2018/4/19 18:02:30
 */

#ifndef _SCOPE_GUARD_H
#define _SCOPE_GUARD_H

#include <functional>

namespace ijk {

class scope_guard final {
public:
    scope_guard(std::function<void(void)> &&on_exit)
        : on_exit_(std::move(on_exit)), dismissed_(false) {}

    scope_guard(const std::function<void(void)> &on_exit)
        : on_exit_(on_exit), dismissed_(false) {}

    ~scope_guard() {
        if (!dismissed_) on_exit_();
    }

    void dismiss() { dismissed_ = true; }

private:
    std::function<void(void)> on_exit_;
    bool dismissed_;
};  // class ScopeGuard

}  // namespace ijk

#endif
