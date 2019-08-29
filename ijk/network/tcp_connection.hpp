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
#include "ijk/base/logging.hpp"

#include <cassert>
#include <list>
#include <string_view>

namespace ijk {
class tcp_connection final
    : public std::enable_shared_from_this<tcp_connection> {
public:
    using ptr = std::shared_ptr<tcp_connection>;
    using weak_ptr = std::weak_ptr<tcp_connection>;
    using read_callback =
        std::function<size_t(const ptr &, const std::string_view &)>;
    using close_callback =
        std::function<void(const ptr &, const asio::error_code &)>;

    static ptr create(io_t &io) { return std::make_shared<tcp_connection>(io); }

    static ptr create(io_t &io, asio::ip::tcp::socket &&socket) {
        return std::make_shared<tcp_connection>(io, std::move(socket));
    }

    tcp_connection(io_t &io)
        : id_(++next_session_id_),
          io_(io),
          socket_(io_.context()),
          closing_(false),
          closed_(false) {}

    tcp_connection(io_t &io, asio::ip::tcp::socket &&socket)
        : id_(++next_session_id_),
          io_(io),
          socket_(std::move(socket)),
          closing_(false),
          closed_(false) {}

    ~tcp_connection() {}

    inline uint64_t id() { return id_; }

    inline asio::ip::tcp::socket &socket() {
        return socket_;
    }  // not thread safe

    // not thread safe
    std::string local_address() {
        if (!socket_.is_open()) return std::string();
        auto ep = socket_.local_endpoint();
        return ep.address().to_string() + ":" + std::to_string(ep.port());
    }

    std::string remote_address() {  // not thread safe
        if (!socket_.is_open()) return std::string();
        auto ep = socket_.remote_endpoint();
        return ep.address().to_string() + ":" + std::to_string(ep.port());
    }

    void start() {
        Expects(on_read_ != nullptr);
        if (io_.running_in_this_thread()) {
            post_read();
        } else {
            io_.post([this, self = shared_from_this()]() { post_read(); });
        }
    }

    void shutdown() {
        io_.dispatch([this, self = shared_from_this()]() {
            if (is_closing_or_closed()) return;
            closing_ = true;
            if (!is_sending()) {
                do_close(self, asio::error_code());
            }
        });
    }

    void force_shutdown() {
        io_.dispatch([this, self = shared_from_this()]() {
            if (is_closing_or_closed()) return;
            do_close(self, asio::error_code());
        });
    }

    void send(const std::string_view &data) {
        if (is_closing_or_closed()) return;
        if (io_.running_in_this_thread()) {
            do_send(data);
            return;
        }

        io_.post([this, self = shared_from_this(),
                  d = std::string(data.data(), data.size())]() {
            do_send(std::move(d));
        });
    }

    void send(std::string &&data) {
        if (is_closing_or_closed()) return;
        if (io_.running_in_this_thread()) {
            do_send(std::move(data));
            return;
        }

        io_.post([this, self = shared_from_this(), d = std::move(data)]() {
            do_send(std::move(d));
        });
    }

    tcp_connection &on_read(read_callback &&cb) {
        // Expects(io_.running_in_this_thread());
        on_read_ = std::forward<read_callback>(cb);
        return *this;
    }

    tcp_connection &on_closed(close_callback &&cb) {
        // Expects(io_.running_in_this_thread());
        on_closed_ = std::forward<close_callback>(cb);
        return *this;
    }

private:
    template <typename T>
    struct IsStringType {
        enum {
            Value = std::is_same<typename std::decay<T>::type, std::string>::value ||
                    std::is_same<typename std::decay<T>::type, std::string_view>::value,
        };
    };

    template <typename T, typename U = typename std::decay<T>::type,
              typename std::enable_if<IsStringType<U>::Value>::type * = nullptr>
    inline void do_send(T &&data) {
        if (is_closing_or_closed()) return;

        if constexpr (std::is_same<U, std::string_view>::value) {
            send_queue_.push_back(std::string(data.data(), data.size()));
        } else if constexpr (std::is_same<U, std::string>::value) {
            send_queue_.push_back(std::forward<T>(data));
        }

        if (sending_queue_.empty()) post_write();
    }

    void post_read() {
        assert(io_.running_in_this_thread());
        if (!socket_.is_open()) {
            LOG_WARN("socket was already close, session {}", id());
            return;
        }
        recv_buf_.reserve(4096);
        socket_.async_read_some(
            asio::buffer(recv_buf_.writable_head(),
                         recv_buf_.writeable_bytes()),
            [this, self = shared_from_this()](auto &ec, auto bytes_transfered) {
                if (ec) {
                    do_close(self, ec);
                    return;
                }
                recv_buf_.commit(bytes_transfered);
                if (on_read_) {
                    size_t nbytes = on_read_(
                        self, std::string_view((char *)recv_buf_.data(),
                                               recv_buf_.size()));
                    recv_buf_.consume(nbytes);
                }
                post_read();
            });
    }

    void post_write() {
        assert(io_.running_in_this_thread());
        sending_buffers_.clear();
        sending_queue_.clear();
        if (send_queue_.empty()) {
            if (closing_) {
                LOG_DEBUG("close session after send completed, id {}", id());
                do_close(shared_from_this(), asio::error_code());
            }
            return;
        }

        if (!socket_.is_open()) return;
        sending_queue_.swap(send_queue_);
        for (const auto &data : sending_queue_) {
            sending_buffers_.emplace_back(
                asio::buffer(data.data(), data.size()));
        }

        asio::async_write(
            socket_, sending_buffers_,
            [this, self = shared_from_this()](auto &ec, auto) {
                if (ec) {
                    do_close(self, ec);
                    return;
                }
                post_write();
            });
    }

    void do_close(const ptr &self, const asio::error_code &ec) {
        assert(io_.running_in_this_thread());
        closing_ = false;
        closed_ = true;
        if (on_closed_) on_closed_(self, ec);
        on_closed_ = nullptr;
        on_read_ = nullptr;
        if (socket_.is_open()) {
            asio::error_code ignored_ec;
            socket_.shutdown(asio::socket_base::shutdown_both, ignored_ec);
            socket_.close(ignored_ec);
        }
    }

    bool is_sending() {
        return !(sending_queue_.empty() && send_queue_.empty());
    }

    bool is_closing_or_closed() { return closed_ || closing_; }

private:
    inline static std::atomic_uint64_t next_session_id_{0};
    uint64_t id_;
    io_t &io_;
    asio::ip::tcp::socket socket_;
    buffer recv_buf_;
    std::list<std::string> send_queue_;
    std::list<std::string> sending_queue_;
    std::vector<asio::const_buffer> sending_buffers_;
    read_callback on_read_;
    close_callback on_closed_;
    bool closing_;
    bool closed_;
};

}  // namespace ijk

#endif /* end of include guard: TCP_SESSION_H_2RZWVGWL */
