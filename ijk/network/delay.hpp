/*
 * File  : delay.hpp
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/08/30 19:20:28
 */

#pragma once
#ifndef DELAY_HPP_ETVNSJMB
#define DELAY_HPP_ETVNSJMB

#include "ijk/future/future.h"
#include "ijk/network/io_context.hpp"

#include <chrono>

namespace ijk {

future<void> delay(io_t &io, const std::chrono::nanoseconds &ns) {
    promise<void> pm;
    auto fut = pm.get_future();

    auto timer = std::make_shared<asio::steady_timer>(io.context());
    timer->expires_after(ns);
    timer->async_wait([timer, pm = std::move(pm)](auto &ec) mutable {
        if (!ec) {
            pm.set_value();
        }
    });

    return fut;
}

future<void> delay(asio::steady_timer &timer, const std::chrono::nanoseconds &ns) {
    using storage_type = future<void>::storage_type;
    detail::Storage_ptr<storage_type> storage;
    storage.allocate(storage_type::allocator_type{});

    timer.expires_after(ns);
    timer.async_wait([storage](auto &ec) mutable {
        if (!ec) {
            storage->fullfill(storage_type::fullfill_type{});
        }
    });

    return storage_type::future_type{storage};
}

}  // namespace ijk

#endif /* end of include guard: DELAY_HPP_ETVNSJMB */
