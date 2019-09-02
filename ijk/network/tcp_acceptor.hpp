/*
 * File  : tcp_service.h
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/07/07 22:22:52
 */

#pragma once
#ifndef TCP_SERVICE_H_PZITAISO
#define TCP_SERVICE_H_PZITAISO

#include "io_context.hpp"

#include "ijk/base/cancel_token.h"
#include "ijk/base/gsl.h"
#include "ijk/base/logging.hpp"
#include "ijk/base/noncopyable.h"

namespace ijk {

class tcp_acceptor final : public noncopyable {
public:
    using accept_callback =
        std::function<void(asio::ip::tcp::socket &&, io_t &io)>;

    tcp_acceptor(io_t &io, const asio::ip::tcp::endpoint &ep,
                 io_context_pool *io_pool = nullptr)
        : io_(io),
          io_pool_(io_pool),
          token_(makeCancelToken()),
          timer_(io.context()) {
        try {
            acceptor_ = std::make_unique<asio::ip::tcp::acceptor>(io_.context(),
                                                                  ep, true);
            acceptor_->listen();
        } catch (const std::exception &e) {
            LOG_ERROR("failed to listen: {}, error: {}", ep, e.what());
        }
    }

    ~tcp_acceptor() = default;

    void start(accept_callback &&cb) {
        accept_cb_ = std::forward<accept_callback>(cb);
        Ensures(accept_cb_);
        do_accept();
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
    io_t *next_io() { return &(io_pool_ != nullptr ? io_pool_->get() : io_); }

    void do_accept() {
        auto io = next_io();
        acceptor_->async_accept(
            io->context(),
            [this, io, wt = WeakCancelToken(token_)](auto &ec, auto socket) {
                if (wt.expired()) return;
                if (ec) {
                    LOG_ERROR("accepter error: {}", ec);
                    timer_.expires_after(std::chrono::seconds(3));
                    timer_.async_wait([this, wt](auto &ec) {
                        if (wt.expired()) return;
                        do_accept();
                    });
                    return;
                }
                accept_cb_(std::move(socket), *io);
                do_accept();
            });
    }

private:
    io_t &io_;
    io_context_pool *io_pool_;
    std::unique_ptr<asio::ip::tcp::acceptor> acceptor_;
    accept_callback accept_cb_;
    asio::steady_timer timer_;
    SharedCancelToken token_;
};
}  // namespace ijk

#endif /* end of include guard: TCP_SERVICE_H_PZITAISO */
