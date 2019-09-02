/*
 * File  : tcp_client.hpp
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/08/03 18:49:12
 */

#pragma once
#ifndef TCP_CLIENT_HPP_B8QMLVHX
#define TCP_CLIENT_HPP_B8QMLVHX

#include "io_context.hpp"
#include "tcp_connection.hpp"

#include "ijk/base/cancel_token.h"
#include "ijk/base/logging.hpp"
#include "ijk/base/noncopyable.h"

#include <atomic>
#include <chrono>

namespace ijk {
class tcp_client : private noncopyable,
                   public std::enable_shared_from_this<tcp_client> {
public:
    using ptr = std::shared_ptr<tcp_client>;
    using connect_callback = std::function<void(const ptr &)>;
    using connecting_callback = std::function<void(const ptr &)>;
    using disconnect_callback =
        std::function<void(const ptr &, const asio::error_code &)>;
    using read_callback =
        std::function<size_t(const ptr &, const std::string_view &data)>;

    static ptr create(io_t &io) {
        class MakeSharedProxy : public tcp_client {
        public:
            MakeSharedProxy(io_t &io) : tcp_client(io) {}
        };
        return std::make_shared<MakeSharedProxy>(io);
    }

    tcp_client &on_read(read_callback &&cb) {
        opts_.on_read = std::forward<read_callback>(cb);
        return *this;
    }

    tcp_client &on_connected(connect_callback &&cb) {
        opts_.on_connected = std::forward<connect_callback>(cb);
        return *this;
    }

    tcp_client &on_disconnected(disconnect_callback &&cb) {
        opts_.on_disconnected = std::forward<disconnect_callback>(cb);
        return *this;
    }

    tcp_client &on_connecting(connecting_callback &&cb) {
        opts_.on_connecting = std::forward<connecting_callback>(cb);
        return *this;
    }

    tcp_client &reconnect_interval(std::chrono::milliseconds t) {
        opts_.reconnect_interval = t;
        return *this;
    }

    tcp_client &connect_timeout(std::chrono::milliseconds t) {
        opts_.connect_timeout = t;
        return *this;
    }

    ~tcp_client() = default;

    template <typename... Opt>
    void connect(const asio::ip::tcp::endpoint &ep) {
        if (status_ == connecting || status_ == connected) {
            LOG_WARN("already connecting or connected");
            return;
        }
        opts_.endpoint = ep;
        do_connect();
    }

    void stop() {
        io_.dispatch([this, wt = WeakCancelToken(token_)]() {
            if (wt.expired()) return;
            stoped = true;
            token_ = makeCancelToken();
            timer_.cancel();
            session_.reset();
            status_ = disconnected;
        });
    }

    bool is_connected() { return status_ == connected; }

    void send(const std::string_view &data) {
        if (io_.running_in_this_thread()) {
            if (!is_connected()) {
                LOG_WARN("client is not connected");
                return;
            }
            Expects(session_);
            session_->send(data);
        } else {
            io_.post(
                [this, self = shared_from_this(), d = std::string(data)]() {
                    if (!is_connected()) {
                        LOG_WARN("client is not connected");
                        return;
                    }
                    session_->send(std::move(d));
                });
        }
    }

private:
    tcp_client(io_t &io)
        : io_(io),
          timer_(io.context()),
          opts_(),
          status_(disconnected),
          stoped(false) {}

    void do_connect() {
        status_ = connecting;
        token_ = makeCancelToken();
        session_ = tcp_connection::create(io_);
        session_->socket().async_connect(
            opts_.endpoint, [this, wt = WeakCancelToken(token_)](auto &ec) {
                if (wt.expired()) return;
                if (connecting != status_) return;
                if (ec) {
                    status_ = disconnected;
                } else {
                    status_ = connected;
                }
                on_status_changed(status_, ec);
            });
        if (opts_.connect_timeout > std::chrono::milliseconds()) {
            timer_.expires_after(opts_.connect_timeout);
            timer_.async_wait([this, wt = WeakCancelToken(token_)](auto &ec) {
                if (wt.expired()) return;
                if (ec == asio::error::operation_aborted) {
                    return;
                }
                if (status_ == connecting) {
                    status_ = disconnected;
                    on_status_changed(status_, asio::error::timed_out);
                }
            });
        }
    }

private:
    void on_status_changed(int s, const asio::error_code &ec) {
        auto self = shared_from_this();
        timer_.cancel();
        if (disconnected == s) {
            session_.reset();
            if (opts_.on_disconnected) opts_.on_disconnected(self, ec);
            if (!stoped &&
                opts_.reconnect_interval > std::chrono::milliseconds())
                reconnect();
        } else if (connected == s) {
            Expects(session_);
            session_
                ->on_read(
                    [this, wt = WeakCancelToken(token_)](auto &, auto &data) {
                        if (wt.expired()) return (size_t)0;
                        return opts_.on_read(shared_from_this(), data);
                    })
                .on_closed(
                    [this, wt = WeakCancelToken(token_)](auto &, auto &ec) {
                        status_ = disconnected;
                        on_status_changed(status_, ec);
                    });
            session_->start();
            if (opts_.on_connected) opts_.on_connected(self);
        } else if (connecting == s) {
            if (opts_.on_connecting) opts_.on_connecting(self);
        }
    }

    void reconnect() {
        timer_.expires_after(opts_.reconnect_interval);
        timer_.async_wait([this, wt = WeakCancelToken(token_)](auto &ec) {
            if (wt.expired()) return;
            if (ec == asio::error::operation_aborted) {
                return;
            }

            do_connect();
        });
        LOG_DEBUG("client will reconnect after {}", opts_.reconnect_interval);
    }

private:
    enum Status {
        disconnected,
        connecting,
        connected,
    };

    struct options {
        asio::ip::tcp::endpoint endpoint;
        std::chrono::milliseconds connect_timeout{0};
        std::chrono::milliseconds reconnect_interval{0};
        read_callback on_read;
        connect_callback on_connected;
        connecting_callback on_connecting;
        disconnect_callback on_disconnected;
    };

    io_t &io_;
    tcp_connection::ptr session_;
    SharedCancelToken token_;
    asio::steady_timer timer_;
    options opts_;
    std::atomic_int status_;
    bool stoped;
};
}  // namespace ijk

#endif /* end of include guard: TCP_CLIENT_HPP_B8QMLVHX */
