/*
 * File Name: sample01.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2018/10/28 15:45:08
 */

#include "ijk/base/logging.hpp"
#include "ijk/network/tcp_client.hpp"

using namespace ijk;

int main(int argc, char *argv[]) {
    IJK_INITIALIZE_LOGGING();
    ijk::io_t io;
    asio::ip::tcp::endpoint ep(asio::ip::address_v4::loopback(), 4000);
    auto client = TcpClient::create(io);
    client
        ->onConnected([](auto &c) {
            LOG_INFO("client is connected");
            c->send("111111");
        })
        .onDisconnected([](auto &, auto &ec) {
            LOG_INFO("cient disconnected with error: {}", ec);
        })
        .onRead([](auto &c, auto &data) {
            c->send(data);
            return data.size();
        })
        .connectTimeout(std::chrono::milliseconds(5000))
        //.reconnectInterval(std::chrono::milliseconds(3000))
        .connect(ep);
    io.run();

    return 0;
}
