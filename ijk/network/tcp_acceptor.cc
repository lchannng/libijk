/*
 * File  : tcp_service.cc
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/07/07 22:22:49
 */

#include "tcp_acceptor.h"

#include "ijk/log/log.h"

namespace ijk {

TcpAcceptor::TcpAcceptor(io_t &io, SessionCreator &&session_creator)
    : io_(io),
      session_creator(std::forward<SessionCreator>(session_creator)),
      acceptor_(io_.context()),
      token_(makeCancelToken()) {}

void TcpAcceptor::start(const asio::ip::tcp::endpoint &ep,
                        AcceptCallback &&cb) {
    if (!io_.running_in_this_thread()) {
        io_.post([this, wt = WeakCancelToken(token_), ep,
                  cb = std::forward<AcceptCallback>(cb)]() mutable {
            if (wt.expired()) return;
            start(ep, std::move(cb));
        });
        return;
    }

    Expects(io_.running_in_this_thread());

    try {
        acceptor_.open(ep.protocol());
        acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(ep);
        acceptor_.listen();
        startAccept(std::move(cb));
    } catch (asio::system_error &e) {
        LOG_ERROR("start error: {}", e.what());
    }
}

void TcpAcceptor::stop() {
    io_.dispatch([this, wt = WeakCancelToken(token_)]() {
        Expects(io_.running_in_this_thread());
        if (wt.expired()) return;
        acceptor_.cancel();
        acceptor_.close();
        token_ = makeCancelToken();
    });
}

void TcpAcceptor::startAccept(AcceptCallback &&cb) {
    assert(io_.running_in_this_thread());
    if (!acceptor_.is_open()) return;
    auto sess = session_creator();
    acceptor_.async_accept(
        sess->socket(),
        [this, wt = WeakCancelToken(token_), sess,
         cb = std::forward<AcceptCallback>(cb)](auto &ec) mutable {
            if (wt.expired()) return;
            if (ec) {
                LOG_ERROR("accepter error: {}", ec);
                return;
            }
            cb(std::move(sess));
            startAccept(std::move(cb));
        });
}

}  // namespace ijk
