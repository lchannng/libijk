/*
 * File  : tcp_service.h
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/07/07 22:22:52
 */

#pragma once
#ifndef TCP_SERVICE_H_PZITAISO
#define TCP_SERVICE_H_PZITAISO

#include "io_context_pool.h"
#include "tcp_session.h"

#include "ijk/base/cancel_token.h"
#include "ijk/base/noncopyable.h"
#include "ijk/base/string_view.h"

namespace ijk {

class TcpAcceptor final : public Noncopyable {
public:
    using AcceptCallback =
        std::function<void(TcpSession::Ptr &&)>;
    TcpAcceptor(io_t &io, io_context_pool &io_pool)
        : io_pool_(io_pool),
          io_(io),
          acceptor_(io_.context()),
          token_(makeCancelToken()) {}
    ~TcpAcceptor() {};
    void start(std::string host, int port, AcceptCallback &&cb);
    void stop();

private:
    void startAccept(AcceptCallback &&cb);

private:
    io_context_pool &io_pool_;
    io_t &io_;
    asio::ip::tcp::acceptor acceptor_;
    SharedCancelToken token_;
};
}

#endif /* end of include guard: TCP_SERVICE_H_PZITAISO */
