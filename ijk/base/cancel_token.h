/*
 * File  : cancel_token.h
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2018/12/05 00:17:39
 */

#ifndef _CANCEL_TOKEN_H
#define _CANCEL_TOKEN_H

#include <memory>

namespace ijk {

namespace details {
struct NoopDeleter {
    void operator()(void *) {}
};
}  // namespace details

using SharedCancelToken = std::shared_ptr<void>;
using WeakCancelToken = std::weak_ptr<void>;

inline SharedCancelToken makeCancelToken() {
    return SharedCancelToken(static_cast<void *>(0), details::NoopDeleter());
}

}  // namespace ijk

#endif
