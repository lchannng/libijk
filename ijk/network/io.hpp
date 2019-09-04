/*
 * File  : io.hpp
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/09/02 17:00:52
 */

#pragma once
#ifndef IO_HPP_FVARH3WU
#define IO_HPP_FVARH3WU

#include "ijk/base/gsl.h"
#include "ijk/future/future.h"
#include "ijk/network/io_context.hpp"

namespace ijk {

future<void> accept(asio::ip::tcp::acceptor &acceptor,
                    asio::ip::tcp::socket &socket) {
    Expects(!socket.is_open());
    promise<void> pm;
    auto fut = pm.get_future();
    acceptor.async_accept(socket, [pm = std::move(pm)](auto &ec) mutable {
        if (ec) {
            pm.set_exception(std::make_exception_ptr(ec));
        } else {
            pm.set_value();
        }
    });
    return fut;
}

template <typename Connection>
future<std::shared_ptr<Connection>> accept(asio::ip::tcp::acceptor &acceptor,
                                           io_t &io) {
    promise<std::shared_ptr<Connection>> pm;
    auto fut = pm.get_future();
    acceptor.async_accept(io.context(), [&io, pm = std::move(pm)](
                                            auto &ec, auto &&socket) mutable {
        if (ec) {
            pm.set_exception(std::make_exception_ptr(ec));
        } else {
            pm.set_value(std::make_shared<Connection>(io, std::move(socket)));
        }
    });
    return fut;
}

template<typename Connection>
future<std::shared_ptr<Connection>> dial(io_t &io, const asio::ip::tcp::endpoint &ep) {
    promise<std::shared_ptr<Connection>> pm;
    auto fut = pm.get_future();

    auto conn = std::make_shared<Connection>(io);
    conn->socket().async_connect(ep, [conn, pm = std::move(pm)](auto &ec) mutable {
        if (!ec) {
            pm.set_value(std::move(conn));
        } else {
            pm.set_exception(std::make_exception_ptr(asio::system_error(ec)));
        }
    });

    return fut;
}

template <typename Connection>
future<std::shared_ptr<Connection>> dial(io_t &io, const std::string &host,
                                         int port) {
    asio::error_code ec;
    auto addr = asio::ip::make_address(host, ec);
    if (!ec) return dial<Connection>(io, asio::ip::tcp::endpoint(addr, port));

    using resolve_result = asio::ip::tcp::resolver::results_type;

    promise<resolve_result> resolve_pm;
    auto resolve_fut = resolve_pm.get_future();

    auto resolver = std::make_shared<asio::ip::tcp::resolver>(io.context());
    resolver->async_resolve(
        host, std::to_string(port),
        [resolver, pm = std::move(resolve_pm)](auto &ec, auto res) mutable {
            if (ec) {
                pm.set_exception(std::make_exception_ptr(ec));
            } else {
                pm.set_value(std::move(res));
            }
        });

    return resolve_fut.then([&io](resolve_result res) {
        promise<std::shared_ptr<Connection>> conn_pm;
        auto conn_fut = conn_pm.get_future();

        auto conn = std::make_shared<Connection>(io);
        asio::async_connect(
            conn->socket(), res,
            [conn, pm = std::move(conn_pm)](auto &ec, auto) mutable {
                if (!ec) {
                    pm.set_value(std::move(conn));
                } else {
                    pm.set_exception(std::make_exception_ptr(ec));
                }
            });

        return conn_fut;
    });
}

future<size_t> read_some(asio::ip::tcp::socket &socket,
                         const asio::mutable_buffer &buf) {
    if (!socket.is_open()) {
        return make_ready_future<size_t>(0);
    }

    promise<size_t> pm;
    auto fut = pm.get_future();
    socket.async_read_some(
        buf, [pm = std::move(pm)](auto &ec, auto bytes_transfered) mutable {
            if (!ec) {
                pm.set_value(bytes_transfered);
                return;
            } else {
                if (asio::error::eof == ec && bytes_transfered > 0) {
                    pm.set_value(bytes_transfered);
                } else {
                    pm.set_exception(std::make_exception_ptr(ec));
                }
            }
        });
    return fut;
}

future<size_t> read_exactly(asio::ip::tcp::socket &socket,
                            const asio::mutable_buffer &buf) {
    if (!socket.is_open()) {
        return make_ready_future<size_t>(0);
    }

    promise<size_t> pm;
    auto fut = pm.get_future();
    asio::async_read(
        socket, buf,
        [pm = std::move(pm)](auto &ec, auto bytes_transfered) mutable {
            if (!ec) {
                pm.set_value(bytes_transfered);
                return;
            } else {
                if (asio::error::eof == ec && bytes_transfered > 0) {
                    pm.set_value(bytes_transfered);
                } else {
                    pm.set_exception(std::make_exception_ptr(ec));
                }
            }
        });
    return fut;
}

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

future<void> delay(asio::steady_timer &timer,
                   const std::chrono::nanoseconds &ns) {
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

#endif /* end of include guard: IO_HPP_FVARH3WU */