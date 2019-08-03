/*
 * File  : tcp_client.hpp
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/08/03 18:49:12
 */

#pragma once
#ifndef TCP_CLIENT_HPP_B8QMLVHX
#define TCP_CLIENT_HPP_B8QMLVHX

#include "io_context_pool.hpp"
#include "tcp_session.hpp"

#include "ijk/base/cancel_token.h"
#include "ijk/base/logging.hpp"
#include "ijk/base/noncopyable.h"

#include <atomic>
#include <chrono>

namespace ijk {
class TcpClient : private Noncopyable, public std::enable_shared_from_this<TcpClient> {
public:
    using Ptr = std::shared_ptr<TcpClient>;
    using ConnectCallback = std::function<void(const Ptr &)>;
    using ConnectingCallback = std::function<void(const Ptr &)>;
    using DisconnectCallback = std::function<void(const Ptr &, const asio::error_code &)>;
    using ReadCallback = std::function<size_t(const Ptr &, const std::string_view &data)>;

    static Ptr create(io_t &io) {
        class MakeSharedProxy : public TcpClient {
        public:
            MakeSharedProxy(io_t &io) : TcpClient(io) {}
        };
        return std::make_shared<MakeSharedProxy>(io);
    }

    TcpClient& onRead(ReadCallback&& cb) {
        opts_.on_read = std::forward<ReadCallback>(cb);
        return *this;
    }

    TcpClient &onConnected(ConnectCallback &&cb) {
        opts_.on_connected = std::forward<ConnectCallback>(cb);
        return *this;
    }

    TcpClient &onDisconnected(DisconnectCallback &&cb) {
        opts_.on_disconnected = std::forward<DisconnectCallback>(cb);
        return *this;
    }

    TcpClient &onConnecting(ConnectingCallback &&cb) {
        opts_.on_connecting = std::forward<ConnectingCallback>(cb);
        return *this;
    }

    TcpClient& reconnectInterval(std::chrono::milliseconds t) {
        opts_.reconnect_interval = t;
        return *this;
    }

    TcpClient& connectTimeout(std::chrono::milliseconds t) {
        opts_.connect_timeout = t;
        return *this;
    }

    ~TcpClient() = default;

    template <typename... Opt>
    void connect(const asio::ip::tcp::endpoint &ep) {
        if (status_ == connecting || status_ == connected) {
            LOG_WARN("already connecting or connected");
            return;
        }
        opts_.endpoint = ep;
        doConnect();
    }

    void stop() {
        io_.dispatch(
            [this, wt = WeakCancelToken(token_)]() { if (wt.expired()) return;
            stoped = true;
            token_ = makeCancelToken();
            timer_.cancel();
            session_.reset();
            status_ = disconnected;
        });
    }

    bool isConnected() {return false;}

    void send(const std::string_view &data) {
        if (!isConnected()) {
            LOG_WARN("client is not connected");
            return;
        }
        
    }

private:
    TcpClient(io_t &io)
        : io_(io),
          timer_(io.context()),
          opts_(),
          status_(disconnected),
          stoped(false) {} 

    void doConnect() {
        status_ = connecting;
        token_ = makeCancelToken();
        session_ = std::make_shared<TcpSession>(io_);
        session_->socket().async_connect(
            opts_.endpoint, [this, wt = WeakCancelToken(token_)](auto &ec) {
                if (wt.expired()) return;
                if (connecting != status_) return;
                if (ec) {
                    status_ = disconnected;
                } else {
                    status_ = connected;
                }
                onStatusChanged(status_, ec);
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
                    onStatusChanged(status_, asio::error::timed_out);
                }
            });
        }
    }

private:
    void onStatusChanged(int s, const asio::error_code &ec) {
        auto self = shared_from_this();
        timer_.cancel();
        if (disconnected == s) {
            session_.reset();
            if (opts_.on_disconnected) opts_.on_disconnected(self, ec);
            if (!stoped && opts_.reconnect_interval > std::chrono::milliseconds())
                reconnect();
        } else if (connected == s) {
            Expects(session_);
            session_
                ->onRead(
                    [this, wt = WeakCancelToken(token_)](auto &, auto &data) {
                        if (wt.expired()) return (size_t)0;
                        return opts_.on_read(shared_from_this(), data);
                    })
                .onClosed(
                    [this, wt = WeakCancelToken(token_)](auto &, auto &ec) {
                        status_ = disconnected;
                        onStatusChanged(status_, ec);
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

            doConnect();
        });
        LOG_DEBUG("client will reconnect after {}", opts_.reconnect_interval);
    }

private:
    enum Status {
        disconnected,
        connecting,
        connected,
    };

    struct TcpClientOptions {
        asio::ip::tcp::endpoint endpoint;
        std::chrono::milliseconds connect_timeout{0};
        std::chrono::milliseconds reconnect_interval{0};
        ReadCallback on_read;
        ConnectCallback on_connected;
        ConnectingCallback on_connecting;
        DisconnectCallback on_disconnected;
    };

    io_t &io_;
    TcpSession::Ptr session_;
    SharedCancelToken token_;
    asio::steady_timer timer_;
    TcpClientOptions opts_;
    std::atomic_int status_;
    bool stoped;
};
}


#endif /* end of include guard: TCP_CLIENT_HPP_B8QMLVHX */
