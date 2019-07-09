/*
 * File  : tcp_session.h
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/07/09 19:23:21
 */

#pragma once
#ifndef TCP_SESSION_H_2RZWVGWL
#define TCP_SESSION_H_2RZWVGWL

#include "io_context_pool.h"

#include "ijk/base/string_view.h"

namespace ijk {
class TcpSession : public std::enable_shared_from_this<TcpSession> {
public:
    using Ptr = std::shared_ptr<TcpSession>;
    using WeakPtr = std::weak_ptr<TcpSession>;

    using ReadHandler = std::function<void (const asio::error_code &, size_t)>;

    TcpSession(io_t &io, asio::ip::tcp::socket &&socket);
    virtual ~TcpSession() {}
    uint32_t id();
    std::string localAddress();
    std::string remoteAddress();

    void read(asio::mutable_buffer buf, ReadHandler &&h);

    void readSome(asio::mutable_buffer buf, ReadHandler &&h);

    void send(const string_view &data);

protected:
    asio::ip::tcp::socket &socket() { return socket_; }

protected:
    static std::atomic_uint32_t next_session_id_;
    uint32_t id_;
    io_t &io_;
    asio::ip::tcp::socket socket_{io_.context()};
};
}  // namespace ijk

#endif /* end of include guard: TCP_SESSION_H_2RZWVGWL */
