/*
 * File  : tcp_connector.cc
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/07/11 15:08:06
 */

#include "tcp_connector.h"

#include "ijk/base/gsl.h"

namespace ijk {

TcpConnector::TcpConnector(io_t &io)
    : io_(io), resolver_(io_.context()), token_(makeCancelToken()) {}

void TcpConnector::start(const std::string &host, int port,
                         ConnectCallback &&cb) {
    Expects(port >= 0 && port <= 65535);
    if (!io_.running_in_this_thread()) {
        asio::post(io_.strand(),
                   [this, wt = WeakCancelToken(token_), host, port,
                    cb = std::forward<ConnectCallback>(cb)]() mutable {
                       start(host, port, std::move(cb));
                   });
       return;
    }

    Expects(io_.running_in_this_thread());
    asio::ip::tcp::resolver::query query(host, std::to_string(port));
    resolver_.async_resolve(
        query,
        asio::bind_executor(io_.strand(),
                            [this, wt = WeakCancelToken(token_),
                             cb = std::move(cb)](auto &ec, auto iter) mutable {
                                if (wt.expired()) return;
                                resolving_ = false;
                                if (ec) {
                                    cb(ec, nullptr);
                                    return;
                                }
                                auto sess = std::make_shared<TcpSession>(io_);
                                startConnect(sess, std::move(cb), iter);
                            }));
    resolving_ = true;
}

void TcpConnector::stop() {
    asio::dispatch([this, wt = WeakCancelToken(token_)](){
        if (resolving_) {
            resolving_ = false;
            resolver_.cancel();
        }
        token_ = makeCancelToken();
    });
}

void TcpConnector::startConnect(const TcpSession::Ptr &sess,
                                ConnectCallback &&cb,
                                asio::ip::tcp::resolver::iterator iter) {
    Expects(sess != nullptr);
    Expects(io_.running_in_this_thread());
    if (iter == asio::ip::tcp::resolver::iterator()) {
        cb(asio::error::host_unreachable, nullptr);
        return;
    }

    sess->socket().async_connect(
        *iter,
        asio::bind_executor(
            io_.strand(),
            [this, wt = WeakCancelToken(token_), sess = sess,
             cb = std::forward<ConnectCallback>(cb), iter](auto &ec) mutable {
                if (wt.expired()) return;
                if (ec) {
                    startConnect(sess, std::move(cb), ++iter);
                    return;
                }
                cb(asio::error_code(), std::move(sess));
            }));
}

}  // namespace ijk
