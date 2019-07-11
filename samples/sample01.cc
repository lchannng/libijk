/*
 * File Name: sample01.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2018/10/28 15:45:08
 */

#include "ijk/network/asio.h"
#include "ijk/network/tcp_acceptor.h"
#include "ijk/log/logger.h"

using namespace ijk;

int main(int argc, char *argv[]) {
    IJK_INITIALIZE_LOGGING();
    ijk::io_context_pool pool;
    TcpAcceptor acceptor_(pool.get(0), pool);
    acceptor_.start("127.0.0.1", 4000, [](TcpSession::Ptr &&sess) {
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
