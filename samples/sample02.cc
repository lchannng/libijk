/*
 * File Name: sample01.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2018/10/28 15:45:08
 */

#include "ijk/base/logging.hpp"
#include "ijk/network/tcp_connector.h"

using namespace ijk;

int main(int argc, char *argv[]) {
    IJK_INITIALIZE_LOGGING();
    ijk::io_context_pool pool;
    TcpConnector connector(pool.get(0));
    connector.start("127.0.0.1", 4000, [](auto &ec, auto &&sess) {
        if (ec) {
            LOG_ERROR("failed to connect server, error: {}", ec);
            return;
        }
        sess->onRead([](auto &s, auto &data) {
                s->send(data);
                return data.size();
            })
            .onClosed([](auto &s, auto &ec) {
                // LOG_INFO("session {} closed: {}", s->id(), ec);
            })
            .start();
        sess->send("hello,world.");
    });
    pool.run();

    return 0;
}
