/*
 * File  : tcp_service.cc
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/07/07 22:22:49
 */

#include "tcp_acceptor.h"

#include "ijk/log/log.h"

namespace ijk {

TcpAcceptor::TcpAcceptor(io_t &io, io_context_pool &io_pool)
    : io_pool_(io_pool),
      io_(io),
      acceptor_(io_.context()),
      token_(makeCancelToken()) {}

void TcpAcceptor::start(std::string host, int port, AcceptCallback &&cb) {
    Expects(port >= 0 && port <= 65535);

    if (!io_.running_in_this_thread()) {
        asio::post(io_.strand(),
                   [this, wt = WeakCancelToken(token_), host = std::move(host),
                    port, cb = std::forward<AcceptCallback>(cb)]() mutable {
                       if (wt.expired()) return;
                       start(std::move(host), port, std::move(cb));
                   });
        return;
    }

    try {
        asio::ip::tcp::resolver resolver(io_.context());
        auto res = resolver.resolve(host, std::to_string(port));
        auto entry = res.begin();
        acceptor_.open(entry->endpoint().protocol());
        acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(entry->endpoint());
        acceptor_.listen();
        startAccept(std::move(cb));
    } catch(asio::system_error &e) {
        LOG_ERROR("start error: {}", e.what());
    }
}

void TcpAcceptor::stop() {
    asio::dispatch(io_.strand(), [this, wt = WeakCancelToken(token_)]() {
        if (wt.expired()) return;
        acceptor_.cancel();
        acceptor_.close();
        token_ = makeCancelToken();
    });
}

void TcpAcceptor::startAccept(AcceptCallback &&cb) {
    if (!acceptor_.is_open()) return;
    auto sess = std::make_shared<TcpSession>(io_pool_.get());
    acceptor_.async_accept(
        sess->socket(),
        asio::bind_executor(
            io_.strand(),
            [this, wt = WeakCancelToken(token_), sess,
             cb = std::forward<AcceptCallback>(cb)](auto &ec) mutable {
                if (wt.expired()) return;
                if (ec) {
                    LOG_ERROR("accepter error: {}", ec);
                    return;
                }
                cb(std::move(sess));
                startAccept(std::move(cb));
            }));
}

}
