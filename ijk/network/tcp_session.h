/*
 * File  : tcp_session.h
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/07/09 19:23:21
 */

#pragma once
#ifndef TCP_SESSION_H_2RZWVGWL
#define TCP_SESSION_H_2RZWVGWL

#include "buffer.h"
#include "io_context_pool.h"

#include "ijk/base/string_view.h"

namespace ijk {
class TcpSession final : public std::enable_shared_from_this<TcpSession> {
public:
    using Ptr = std::shared_ptr<TcpSession>;
    using WeakPtr = std::weak_ptr<TcpSession>;
    using ReadCallback =
        std::function<size_t(const Ptr &, const string_view &)>;
    using CloseCallback =
        std::function<void(const Ptr &, const asio::error_code &)>;

    TcpSession(io_t &io);
    ~TcpSession();
    inline uint64_t id() { return id_; }

    inline asio::ip::tcp::socket &socket() { return socket_; } // not thread safe
    std::string localAddress(); // not thread safe
    std::string remoteAddress(); // not thread safe

    void start();
    void stop();
    void send(const string_view &data);
    TcpSession &onRead(ReadCallback &&cb);
    TcpSession &onClosed(CloseCallback &&cb);

private:
    void postRead();
    void postWrite();
    void onClose(const Ptr &self, const asio::error_code &ec);

private:
    static std::atomic_uint64_t next_session_id_;
    uint64_t id_;
    io_t &io_;
    asio::ip::tcp::socket socket_;
    Buffer recv_buf_;
    std::list<std::string> send_queue_;
    std::list<std::string> sending_queue_;
    std::vector<asio::const_buffer> sending_buffers_;
    ReadCallback on_read_;
    CloseCallback on_closed_;
    bool writing_;
};
}  // namespace ijk

#endif /* end of include guard: TCP_SESSION_H_2RZWVGWL */
