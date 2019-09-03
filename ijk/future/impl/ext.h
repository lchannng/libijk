/*
 * File  : ext.h
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/09/03 14:11:37
 */

#pragma once
#ifndef EXT_H_NXKYPJ1M
#define EXT_H_NXKYPJ1M

namespace ijk {
template <typename T>
future<T> make_ready_future(T &&val) {
    using storage_type = future<T>::storage_type;
    detail::Storage_ptr<storage_type> storage;
    storage.allocate(storage_type::allocator_type{});
    storage->fullfill(storage_type::fullfill_type{std::forward<T>(val)});
    return storage_type::future_type{storage};
}

future<void> make_ready_future() {
    using storage_type = future<void>::storage_type;
    detail::Storage_ptr<storage_type> storage;
    storage.allocate(storage_type::allocator_type{});
    storage->fullfill(storage_type::fullfill_type{});
    return storage_type::future_type{storage};
}

template <typename T, typename E>
future<T> make_exception_future(E &&e) {
    using storage_type = future<T>::storage_type;
    detail::Storage_ptr<storage_type> storage;
    storage.allocate(storage_type::allocator_type{});
    auto ep = std::make_exception_ptr(std::forward<E>(e));
    storage->fail(storage_type::fail_type{std::move(ep)});
    return storage_type::future_type{storage};
}

template <typename T>
future<T> make_exception_future(std::exception_ptr &&e) {
    using storage_type = future<T>::storage_type;
    detail::Storage_ptr<storage_type> storage;
    storage.allocate(storage_type::allocator_type{});
    storage->fail(storage_type::fail_type{std::move(e)});
    return storage_type::future_type{storage};
}

}

#endif /* end of include guard: EXT_H_NXKYPJ1M */
