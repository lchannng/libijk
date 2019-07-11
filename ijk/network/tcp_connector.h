/*
 * File  : tcp_connector.h
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/07/11 15:08:08
 */

#pragma once
#ifndef TCP_CONNECTOR_H_OCJM7UXK
#define TCP_CONNECTOR_H_OCJM7UXK

#include "io_context_pool.h"
#include "tcp_session.h"

#include "ijk/base/noncopyable.h"
#include "ijk/base/cancel_token.h"

#include <functional>

namespace ijk {
class TcpConnector final : private Noncopyable {
public:
    using ConnectCallback =
        std::function<void(const asio::error_code &, TcpSession::Ptr &&)>;
    TcpConnector(io_t &io);
    ~TcpConnector() = default;
    void start(const std::string &host, int port, ConnectCallback &&cb);
    void stop();

private:
    void startConnect(const TcpSession::Ptr &sess, ConnectCallback &&cb,
                      asio::ip::tcp::resolver::iterator iter);

private:
    io_t &io_;
    asio::ip::tcp::resolver resolver_;
    SharedCancelToken token_;
    bool resolving_ {false};
};
}

#endif /* end of include guard: TCP_CONNECTOR_H_OCJM7UXK */
