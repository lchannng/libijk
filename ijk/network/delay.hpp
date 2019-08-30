/*
 * File  : delay.hpp
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/08/30 19:20:28
 */

#pragma once
#ifndef DELAY_HPP_ETVNSJMB
#define DELAY_HPP_ETVNSJMB

#include "ijk/future/future.h"
#include "ijk/network/io_context_pool.hpp"

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
        } else {
            pm.set_exception(std::make_exception_ptr(asio::system_error(ec)));
        }
    });

    return fut;
}
}  // namespace ijk

#endif /* end of include guard: DELAY_HPP_ETVNSJMB */
