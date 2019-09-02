/*
 * File  : tcp_connector.hpp
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/08/22 10:19:53
 */

#pragma once
#ifndef TCP_CONNECTOR_HPP_CU3Z2MQK
#define TCP_CONNECTOR_HPP_CU3Z2MQK

#include "ijk/base/noncopyable.h"
#include "ijk/base/gsl.h"
#include "ijk/network/io_context.hpp"
#include "ijk/network/tcp_connection.hpp"

#include <functional>
#include <memory>

namespace ijk {

class tcp_connector final : public noncopyable,
                            public std::enable_shared_from_this<tcp_connector> {
public:
    using ptr = std::shared_ptr<tcp_connector>;
    using weak_ptr = std::weak_ptr<tcp_connector>;
    using begin_cb = std::function<void()>;
    using end_cb = std::function<void(const asio::error_code &,
                                      tcp_connection::ptr &&)>;
    static ptr create(io_t &io) { return std::make_shared<tcp_connector>(io); }

    tcp_connector(io_t &io)
        : io_(io), timer_(io.context()), socket_(io.context()) {}

    ~tcp_connector() {}

    tcp_connector &retry_interval(std::chrono::milliseconds t) {
        retry_interval_ = t;
        return *this;
    }

    tcp_connector &on_begin_connecting(begin_cb &&cb) {
        on_begin_ = std::forward<begin_cb>(cb);
        return *this;
    }

    tcp_connector &on_end_connecting(end_cb &&cb) {
        on_end_ = std::forward<end_cb>(cb);
        return *this;
    }

    void connect(const asio::ip::tcp::endpoint &ep) {
        if (io_.running_in_this_thread()) {
            connect_in_io_thread(ep);
        } else {
            io_.post([this, self = shared_from_this(), ep]() {
                connect_in_io_thread(ep);
            });
        }
    }

    void shutdown() {
        if (io_.running_in_this_thread()) {
            shutdown_in_io_thread();
        } else {
            io_.post([this, self = shared_from_this()]() {
                shutdown_in_io_thread();
            });
        }
    }

private:
    void connect_in_io_thread(const asio::ip::tcp::endpoint& ep) {
        Expects(on_end_);
        if (connecting_) {
            LOG_WARN("tcp connector was already connecting, ep: {}", ep);
            return;
        }
        socket_.async_connect(ep, [this, ep, self = shared_from_this()](auto &ec) {
            timer_.cancel();
            connecting_ = false;

            if (closed_) return;

            if (!ec) {
                on_end_(ec, tcp_connection::create(io_, std::move(socket_)));
                return;
            } else {
                on_end_(ec, nullptr);
            }

            if (!closed_ && retry_interval_ > std::chrono::milliseconds(0)) {
                retry(ep);
                LOG_DEBUG("retry connecting to {} after {}", ep, retry_interval_);
            }
        });

        connecting_ = true;

        if (on_begin_) on_begin_();
    }

    void shutdown_in_io_thread() {
        if (closed_) return;
        timer_.cancel();
        if (socket_.is_open()) {
            asio::error_code ec;
            socket_.cancel(ec);
            socket_.close(ec);
        }
    }

    void retry(const asio::ip::tcp::endpoint &ep) {
        timer_.expires_after(retry_interval_);
        timer_.async_wait([this, ep, self = shared_from_this()](auto &ec) {
            if (closed_) return;
            if (!ec) connect_in_io_thread(ep);
        });
    }

private:
    io_t &io_;
    asio::steady_timer timer_;
    std::chrono::milliseconds retry_interval_{0};
    begin_cb on_begin_;
    end_cb on_end_;
    asio::ip::tcp::socket socket_;
    bool connecting_{false};
    bool closed_ {false};
};

}  // namespace ijk

#endif /* end of include guard: TCP_CONNECTOR_HPP_CU3Z2MQK */
