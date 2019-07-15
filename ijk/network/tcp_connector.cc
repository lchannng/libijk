/*
 * File  : tcp_connector.cc
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/07/11 15:08:06
 */

#include "tcp_connector.h"

#include "ijk/base/gsl.h"

namespace ijk {

TcpConnector::TcpConnector(io_t &io)
    : io_(io), token_(makeCancelToken()) {}

void TcpConnector::start(const std::string &host, int port,
                         ConnectCallback &&cb) {
    Expects(port >= 0 && port <= 65535);
    if (!io_.running_in_this_thread()) {
        io_.post([this, wt = WeakCancelToken(token_), host, port,
                  cb = std::forward<ConnectCallback>(cb)]() mutable {
            start(host, port, std::move(cb));
        });
        return;
    }

    Expects(io_.running_in_this_thread());

    asio::error_code ec;
    asio::ip::tcp::resolver resolver(io_.context());
    auto res = resolver.resolve(host, std::to_string(port), ec);
    if (ec) {
        cb(ec, nullptr);
        return;
    }

    auto sess = std::make_shared<TcpSession>(io_);
    asio::async_connect(
        sess->socket(), res.begin(), res.end(),
        [this, wt = WeakCancelToken(token_), sess = sess,
         cb = std::forward<ConnectCallback>(cb)](auto &ec, auto ep) mutable {
            if (wt.expired()) return;
            if (ec) {
                cb(ec, nullptr);
                return;
            }
            cb(asio::error_code(), std::move(sess));
        });
}

void TcpConnector::stop() {
    asio::dispatch([this, wt = WeakCancelToken(token_)](){
        token_ = makeCancelToken();
    });
}

}  // namespace ijk
