/*
 * File  : dial.hpp
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/08/30 15:05:37
 */

#pragma once
#ifndef DIAL_HPP_EP2ZWQFZ
#define DIAL_HPP_EP2ZWQFZ

#include "ijk/future/future.h"
#include "ijk/network/tcp_connection.hpp"

namespace ijk {

future<tcp_connection::ptr> dial(io_t &io, const asio::ip::tcp::endpoint &ep) {
    promise<tcp_connection::ptr> pm;
    auto fut = pm.get_future();

    auto conn = tcp_connection::create(io);
    conn->socket().async_connect(ep, [conn, pm = std::move(pm)](auto &ec) mutable {
        if (!ec) {
            pm.set_value(std::move(conn));
        } else {
            pm.set_exception(std::make_exception_ptr(asio::system_error(ec)));
        }
    });

    return fut;
}

future<tcp_connection::ptr> dial(io_t &io, const std::string &host, int port) {
    asio::error_code ec;
    auto addr = asio::ip::make_address(host, ec);
    if (!ec) return dial(io, asio::ip::tcp::endpoint(addr, port));

    using resolve_result = asio::ip::tcp::resolver::results_type;

    promise<resolve_result> resolve_pm;
    auto resolve_fut = resolve_pm.get_future();

    auto resolver = std::make_shared<asio::ip::tcp::resolver>(io.context());
    resolver->async_resolve(
        host, std::to_string(port),
        [resolver, pm = std::move(resolve_pm)](auto &ec, auto res) mutable {
            if (ec) {
                pm.set_exception(
                    std::make_exception_ptr(asio::system_error(ec)));
            } else {
                pm.set_value(std::move(res));
            }
        });

    promise<tcp_connection::ptr> conn_pm;
    auto conn_fut = conn_pm.get_future();

    resolve_fut.finally([pm = std::move(conn_pm),
                         &io](expected<resolve_result> e) mutable {
        if (!e.has_value()) {
            pm.set_exception(e.error());
            return;
        }

        auto conn = tcp_connection::create(io);
        asio::async_connect(conn->socket(), e.value(),
                            [conn, pm = std::move(pm)](auto &ec, auto) mutable {
                                if (!ec) {
                                    pm.set_value(std::move(conn));
                                } else {
                                    pm.set_exception(std::make_exception_ptr(
                                        asio::system_error(ec)));
                                }
                            });
    });

    return conn_fut;
}

}

#endif /* end of include guard: DIAL_HPP_EP2ZWQFZ */
