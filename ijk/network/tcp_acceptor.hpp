/*
 * File  : tcp_service.h
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/07/07 22:22:52
 */

#pragma once
#ifndef TCP_SERVICE_H_PZITAISO
#define TCP_SERVICE_H_PZITAISO

#include "io_context_pool.hpp"
#include "tcp_session.hpp"

#include "ijk/base/cancel_token.h"
#include "ijk/base/gsl.h"
#include "ijk/base/logging.hpp"
#include "ijk/base/noncopyable.h"

namespace ijk {

class TcpAcceptor final : public noncopyable {
public:
    using AcceptCallback =
        std::function<void(TcpSession::Ptr &&)>;

    TcpAcceptor(io_t &io, const asio::ip::tcp::endpoint &ep,
                io_context_pool *io_pool = nullptr)
        : io_(io), io_pool_(io_pool), token_(makeCancelToken()), timer_(io.context()) {
        try {
            acceptor_ = std::make_unique<asio::ip::tcp::acceptor>(io_.context(), ep, true);
            acceptor_->listen();
        } catch (const std::exception &e) {
            LOG_ERROR("failed to listen: {}, error: {}", ep, e.what());
        }
    }

    ~TcpAcceptor() = default;
    void start(AcceptCallback &&cb) {
        accept_cb_ = std::forward<AcceptCallback>(cb);
        Ensures(accept_cb_);
        doAccept();
    }

    void stop() {
        io_.dispatch([this, wt = WeakCancelToken(token_)]() {
            Expects(io_.running_in_this_thread());
            if (wt.expired()) return;
            acceptor_->cancel();
            acceptor_->close();
            timer_.cancel();
            token_ = makeCancelToken();
            accept_cb_ = nullptr;
        });
    }

private:
    io_t* nextIo() {
        return &(io_pool_ != nullptr ? io_pool_->get() : io_);
    }

    void doAccept() {
        auto io = nextIo();
        acceptor_->async_accept(io->context(), [this, io, wt = WeakCancelToken(token_)](auto &ec, auto socket) {
            if (wt.expired()) return;
            if (ec) {
                LOG_ERROR("accepter error: {}", ec);
                timer_.expires_after(std::chrono::seconds(3));
                timer_.async_wait([this, wt](auto &ec) {
                    if (wt.expired()) return;
                    doAccept();
                });
                return;
            }
            auto sess = std::make_shared<TcpSession>(*io, std::move(socket));
            accept_cb_(std::move(sess));
            doAccept();
        });
    }

private:
    io_t &io_;
    io_context_pool *io_pool_;
    std::unique_ptr<asio::ip::tcp::acceptor> acceptor_;
    AcceptCallback accept_cb_;
    asio::steady_timer timer_;
    SharedCancelToken token_;
};
}

#endif /* end of include guard: TCP_SERVICE_H_PZITAISO */
