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
class TcpSession {
public:
    int64_t id();
    std::string localAddress();
    std::string remoteAddress();

    void start();
    void send(const string_view &data);

protected:
    auto &socket() { return socket_; }

protected:
    io_t &io_;
    asio::ip::tcp::socket socket_{io_.context()};
};
}  // namespace ijk

#endif /* end of include guard: TCP_SESSION_H_2RZWVGWL */
