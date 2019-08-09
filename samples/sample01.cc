/*
 * File Name: sample01.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2018/10/28 15:45:08
 */

#include "ijk/base/logging.hpp"
#include "ijk/network/asio_headers.hpp"
#include "ijk/network/tcp_acceptor.hpp"
#include "ijk/network/tcp_session.hpp"

using namespace ijk;

int main(int argc, char *argv[]) {
    IJK_INITIALIZE_LOGGING();
    ijk::io_context_pool pool;
    asio::ip::tcp::endpoint ep(asio::ip::address_v4::loopback(), 4000);

    LOG_INFO("server start at {}", ep);
    tcp_acceptor acceptor(pool.get(0), ep, &pool);
    acceptor.start([](auto &&socket, auto &io) {
        auto sess = std::make_shared<tcp_session>(io, std::move(socket));
        sess->on_read([](auto &s, auto &data) {
                s->send(data);
                return data.size();
            })
            .on_closed([](auto &s, auto &ec) {
                // LOG_INFO("session {} closed: {}", s->id(), ec);
            })
            .start();
    });


    pool.run();

    return 0;
}
