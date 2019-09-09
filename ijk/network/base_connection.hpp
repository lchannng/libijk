/*
 * File  : base_connection.hpp
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/09/05 15:06:28
 */

#pragma once
#ifndef BASE_CONNECTION_HPP_2NGLFYCW
#define BASE_CONNECTION_HPP_2NGLFYCW

#include "ijk/base/gsl.h"
#include "ijk/base/logging.hpp"
#include "ijk/base/noncopyable.h"
#include "ijk/network/io.hpp"

#include <cassert>
#include <list>
#include <string_view>

namespace ijk {

namespace details {
struct abstract_connection : private noncopyable {
    virtual void run() = 0;
    virtual ~abstract_connection() = default;
};
}  // namespace details

template <typename Derived>
class base_connection : public std::enable_shared_from_this<Derived>,
                        public details::abstract_connection {
public:
    using ptr = std::shared_ptr<Derived>;
    using weak_ptr = std::weak_ptr<Derived>;
    using message_callback =
        std::function<size_t(const ptr &, const std::string_view &)>;
    using close_callback =
        std::function<void(const ptr &, const asio::error_code &)>;

    base_connection(io_t &io)
        : id_(++next_session_id_),
          io_(io),
          socket_(io_.context()),
          closing_(false),
          closed_(false) {}

    base_connection(io_t &io, asio::ip::tcp::socket &&socket)
        : id_(++next_session_id_),
          io_(io),
          socket_(std::move(socket)),
          closing_(false),
          closed_(false) {}

    ~base_connection() = default;

    using std::enable_shared_from_this<Derived>::shared_from_this;
    using std::enable_shared_from_this<Derived>::weak_from_this;

    inline uint64_t id() { return id_; }

    inline io_t &io() { return io_; }

    inline asio::ip::tcp::socket &socket() { return socket_; }

    // not thread safe
    std::string local_address() {
        if (!socket_.is_open()) return std::string();
        auto ep = socket_.local_endpoint();
        return ep.address().to_string() + ":" + std::to_string(ep.port());
    }

    // not thread safe
    std::string remote_address() {  // not thread safe
        if (!socket_.is_open()) return std::string();
        auto ep = socket_.remote_endpoint();
        return ep.address().to_string() + ":" + std::to_string(ep.port());
    }

    void shutdown() {
        io_.dispatch([this, self = shared_from_this()]() {
            if (is_closing_or_closed()) return;
            closing_ = true;
            if (!is_sending()) {
                invoke_close_cb(self, asio::error_code());
            }
        });
    }

    void force_shutdown() {
        io_.dispatch([this, self = shared_from_this()]() {
            if (is_closing_or_closed()) return;
            invoke_close_cb(self, asio::error_code());
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

    Derived &on_message(message_callback &&cb) {
        // Expects(io_.running_in_this_thread());
        message_cb_ = std::forward<message_callback>(cb);
        return *static_cast<Derived *>(this);
    }

    Derived &on_close(close_callback &&cb) {
        // Expects(io_.running_in_this_thread());
        close_cb_ = std::forward<close_callback>(cb);
        return *static_cast<Derived *>(this);
    }

    bool is_closing_or_closed() { return closed_ || closing_; }

protected:
    inline void invoke_close_cb(const ptr &self, const asio::error_code &ec) {
        assert(io_.running_in_this_thread());
        closing_ = false;
        closed_ = true;
        if (close_cb_) close_cb_(self, ec);
        close_cb_ = nullptr;
        message_cb_ = nullptr;
        if (socket_.is_open()) {
            asio::error_code ignored_ec;
            socket_.shutdown(asio::socket_base::shutdown_both, ignored_ec);
            socket_.close(ignored_ec);
        }
    }

    inline size_t invoke_message_cb(const ptr &self,
                                    const std::string_view &buf) {
        if (message_cb_) {
            return message_cb_(self, buf);
        }
        return buf.size();
    }

private:
    template <typename T>
    struct IsStringType {
        enum {
            Value = std::is_same<typename std::decay<T>::type,
                                 std::string>::value ||
                    std::is_same<typename std::decay<T>::type,
                                 std::string_view>::value,
        };
    };

    template <typename T, typename U = typename std::decay<T>::type,
              typename std::enable_if<IsStringType<U>::Value>::type * = nullptr>
    inline void do_send(T &&data) {
        if (is_closing_or_closed()) return;

        if constexpr (std::is_same<U, std::string_view>::value) {
            pending_outgoing_messages_.push_back(
                std::string(data.data(), data.size()));
        } else if constexpr (std::is_same<U, std::string>::value) {
            pending_outgoing_messages_.push_back(std::forward<T>(data));
        }

        if (sending_queue_.empty()) write_loop();
    }

    void write_loop() {
        assert(io_.running_in_this_thread());
        sending_buffers_.clear();
        sending_queue_.clear();
        if (pending_outgoing_messages_.empty()) {
            if (closing_) {
                LOG_DEBUG("close connection after send completed, id {}", id());
                invoke_close_cb(shared_from_this(), asio::error_code());
            }
            return;
        }

        if (!socket_.is_open()) return;
        sending_queue_.swap(pending_outgoing_messages_);
        for (const auto &data : sending_queue_) {
            sending_buffers_.emplace_back(
                asio::buffer(data.data(), data.size()));
        }

        asio::async_write(socket_, sending_buffers_,
                          [this, self = shared_from_this()](auto &ec, auto) {
                              if (ec) {
                                  invoke_close_cb(self, ec);
                                  return;
                              }
                              write_loop();
                          });
    }

    bool is_sending() {
        return !(sending_queue_.empty() && pending_outgoing_messages_.empty());
    }

private:
    inline static std::atomic_uint64_t next_session_id_{0};
    uint64_t id_;
    io_t &io_;
    asio::ip::tcp::socket socket_;
    std::list<std::string> pending_outgoing_messages_;
    std::list<std::string> sending_queue_;
    std::vector<asio::const_buffer> sending_buffers_;
    message_callback message_cb_;
    close_callback close_cb_;
    bool closing_;
    bool closed_;
};

}  // namespace ijk

#endif /* end of include guard: BASE_CONNECTION_HPP_2NGLFYCW */
