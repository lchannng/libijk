/*
 * File Name: sample01.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2018/10/28 15:45:08
 */

#include "ijk/base/logging.hpp"
#include "ijk/network/asio_headers.hpp"
#include "ijk/network/tcp_acceptor.h"

using namespace ijk;

int main(int argc, char *argv[]) {
    IJK_INITIALIZE_LOGGING();
    ijk::io_context_pool pool;
    TcpAcceptor acceptor(pool.get(0), [&pool]() {
        return std::make_shared<TcpSession>(pool.get());
    });

    asio::ip::tcp::endpoint ep(asio::ip::address_v4::loopback(), 4000);

    LOG_INFO("server start at {}", ep);

    acceptor.start(ep, [](TcpSession::Ptr &&sess) {
        sess->onRead([](auto &s, auto &data) {
                s->send(data);
                return data.size();
            })
            .onClosed([](auto &s, auto &ec) {
                // LOG_INFO("session {} closed: {}", s->id(), ec);
            })
            .start();
    });
    pool.run();

    return 0;
}
