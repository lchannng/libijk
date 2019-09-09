/*
 * File  : ext.h
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/09/03 14:11:37
 */

#pragma once
#ifndef EXT_H_NXKYPJ1M
#define EXT_H_NXKYPJ1M

namespace ijk {
template <typename... T>
future<T...> make_ready_future(T &&... vals) {
    using future_type = future<T...>;
    using storage_type = typename future_type::storage_type;
    using allocator_type = typename storage_type::allocator_type;
    using fullfill_type = typename storage_type::fullfill_type;

    detail::Storage_ptr<storage_type> storage;
    storage.allocate(allocator_type{});
    storage->fullfill(fullfill_type{std::forward<T>(vals)...});
    return future_type{storage};
}

inline future<void> make_ready_future() {
    using storage_type = typename future<void>::storage_type;
    using allocator_type = typename storage_type::allocator_type;
    using fullfill_type = typename storage_type::fullfill_type;

    detail::Storage_ptr<storage_type> storage;
    storage.allocate(allocator_type{});
    storage->fullfill(fullfill_type{});
    return future<void>{storage};
}

template <typename T, typename E>
future<T> make_exception_future(E &&e) {
    using storage_type = typename future<T>::storage_type;
    using allocator_type = typename storage_type::allocator_type;
    using fail_type = typename storage_type::fail_type;

    detail::Storage_ptr<storage_type> storage;
    storage.allocate(allocator_type{});
    auto ep = std::make_exception_ptr(std::forward<E>(e));
    storage->fail(fail_type{std::move(ep)});
    return future<T>{storage};
}

template <typename T>
future<T> make_exception_future(std::exception_ptr &&e) {
    using storage_type = typename future<T>::storage_type;
    using allocator_type = typename storage_type::allocator_type;
    using fail_type = typename storage_type::fail_type;

    detail::Storage_ptr<storage_type> storage;
    storage.allocate(allocator_type{});
    storage->fail(fail_type{std::move(e)});
    return future<T>{storage};
}

}

#endif /* end of include guard: EXT_H_NXKYPJ1M */
