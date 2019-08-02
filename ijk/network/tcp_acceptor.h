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

namespace ijk {

class TcpAcceptor final : public Noncopyable {
public:
    using AcceptCallback =
        std::function<void(TcpSession::Ptr &&)>;
    using SessionCreator = std::function<TcpSession::Ptr()>;

    TcpAcceptor(io_t &io, SessionCreator &&session_creator);
    ~TcpAcceptor() = default;
    void start(const asio::ip::tcp::endpoint &ep, AcceptCallback &&cb);
    void stop();

private:
    void startAccept(AcceptCallback &&cb);

private:
    io_t &io_;
    SessionCreator session_creator;
    asio::ip::tcp::acceptor acceptor_;
    SharedCancelToken token_;
};
}

#endif /* end of include guard: TCP_SERVICE_H_PZITAISO */
