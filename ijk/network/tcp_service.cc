/*
 * File  : tcp_service.cc
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/07/07 22:22:49
 */

#include "tcp_service.h"

#include "ijk/log/log.h"

namespace ijk {

void TcpAcceptor::start(std::string host, int port, AcceptCallback &&cb) {
    Expects(port >= 0 && port <= 65535);

    if (!io_.running_in_this_thread()) {
        asio::post(io_.strand(),
                   [this, wt = WeakCancelToken(token_), host = std::move(host),
                    port, cb = std::forward<AcceptCallback>(cb)]() mutable {
                       start(std::move(host), port, std::move(cb));
                   });
       return;
    }

    try {
        asio::ip::tcp::resolver resolver(io_.context());
        asio::ip::tcp::resolver::query query(host, std::to_string(port));
        auto iter = resolver.resolve(query);
        asio::ip::tcp::endpoint endpoint = *iter;
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
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
                if (ec) {
                    LOG_ERROR("accepter error: {}", ec);
                    return;
                }
                cb(std::move(sess));
                startAccept(std::move(cb));
            }));
}

}
