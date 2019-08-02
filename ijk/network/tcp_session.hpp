/*
 * File  : tcp_session.h
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/07/09 19:23:21
 */

#pragma once
#ifndef TCP_SESSION_H_2RZWVGWL
#define TCP_SESSION_H_2RZWVGWL

#include "io_context_pool.hpp"

#include "ijk/base/buffer.h"
#include "ijk/base/gsl.h"

#include <cassert>
#include <string_view>

namespace ijk {
class TcpSession final : public std::enable_shared_from_this<TcpSession> {
public:
    using Ptr = std::shared_ptr<TcpSession>;
    using WeakPtr = std::weak_ptr<TcpSession>;
    using ReadCallback =
        std::function<size_t(const Ptr &, const std::string_view &)>;
    using CloseCallback =
        std::function<void(const Ptr &, const asio::error_code &)>;

    TcpSession(io_t &io)
        : id_(++next_session_id_),
          io_(io),
          socket_(io_.context()),
          writing_(false) {}

    ~TcpSession() {}
    inline uint64_t id() { return id_; }

    inline asio::ip::tcp::socket &socket() { return socket_; } // not thread safe

    // not thread safe
    std::string localAddress() {
        if (!socket_.is_open()) return std::string();
        auto ep = socket_.local_endpoint();
        return ep.address().to_string() + ":" + std::to_string(ep.port());
    }

    std::string remoteAddress() { // not thread safe
        if (!socket_.is_open()) return std::string();
        auto ep = socket_.remote_endpoint();
        return ep.address().to_string() + ":" + std::to_string(ep.port());
    }

    void start() {
        Expects(on_read_ != nullptr);
        if (io_.running_in_this_thread()) {
            postRead();
        } else {
            io_.post([this, self = shared_from_this()]() { postRead(); });
        }
    }

    void stop() {
        if (io_.running_in_this_thread()) {
            onClose(shared_from_this(), asio::error_code());
        } else {
            io_.post([this, self = shared_from_this()]() {
                onClose(self, asio::error_code());
            });
        }
    }

    void send(const std::string_view &data) {
        if (io_.running_in_this_thread()) {
            if (!socket_.is_open()) return;
            send_queue_.push_back(std::string(data.data(), data.size()));
            if (sending_queue_.empty());
                postWrite();
            return;
        }

        io_.post([this, self = shared_from_this(),
                  d = std::string(data.data(), data.size())]() {
            if (!socket_.is_open()) return;
            send_queue_.push_back(std::move(d));
            if (sending_queue_.empty()) postWrite();
        });
    }

    TcpSession &onRead(ReadCallback &&cb) {
        // Expects(io_.running_in_this_thread());
        on_read_ = std::forward<ReadCallback>(cb);
        return *this;
    }

    TcpSession &onClosed(CloseCallback &&cb) {
        // Expects(io_.running_in_this_thread());
        on_closed_ = std::forward<CloseCallback>(cb);
        return *this;
    }

private:
    void postRead() {
        assert(io_.running_in_this_thread());
        Expects(socket_.is_open());
        recv_buf_.reserve(4096);
        socket_.async_read_some(
            asio::buffer(recv_buf_.writable_head(),
                         recv_buf_.writeable_bytes()),
            [this, self = shared_from_this()](auto &ec, auto bytes_transfered) {
                if (ec) {
                    onClose(self, ec);
                    return;
                }
                recv_buf_.commit(bytes_transfered);
                if (on_read_) {
                    size_t nbytes = on_read_(
                        self, std::string_view((char *)recv_buf_.data(),
                                               recv_buf_.size()));
                    recv_buf_.consume(nbytes);
                }
                postRead();
            });
    }

    void postWrite() {
        assert(io_.running_in_this_thread());
        sending_buffers_.clear();
        sending_queue_.clear();
        if (send_queue_.empty()) return;
        if (!socket_.is_open()) return;
        sending_queue_.swap(send_queue_);
        for (const auto &data : sending_queue_) {
            sending_buffers_.emplace_back(
                asio::buffer(data.data(), data.size()));
        }

        asio::async_write(
            socket_, sending_buffers_,
            [this, self = shared_from_this()](auto &ec, auto bytes_transfered) {
                if (ec) {
                    onClose(self, ec);
                    return;
                }
                postWrite();
            });
    }

    void onClose(const Ptr &self, const asio::error_code &ec) {
        assert(io_.running_in_this_thread());
        if (on_closed_) on_closed_(self, ec);
        on_closed_ = nullptr;
        on_read_ = nullptr;
        if (socket_.is_open()) {
            asio::error_code ignored_ec;
            socket_.shutdown(asio::socket_base::shutdown_both, ignored_ec);
            socket_.close(ignored_ec);
        }
    }

private:
    inline static std::atomic_uint64_t next_session_id_{0};
    uint64_t id_;
    io_t &io_;
    asio::ip::tcp::socket socket_;
    buffer recv_buf_;
    std::list<std::string> send_queue_;
    std::list<std::string> sending_queue_;
    std::vector<asio::const_buffer> sending_buffers_;
    ReadCallback on_read_;
    CloseCallback on_closed_;
    bool writing_;
};

}  // namespace ijk

#endif /* end of include guard: TCP_SESSION_H_2RZWVGWL */
