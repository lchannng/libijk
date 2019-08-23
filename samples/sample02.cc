/*
 * File Name: sample01.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2018/10/28 15:45:08
 */

#include "ijk/base/logging.hpp"
#include "ijk/network/tcp_client.hpp"
#include "ijk/network/tcp_connector.hpp"

using namespace ijk;

int main(int argc, char *argv[]) {
    IJK_INITIALIZE_LOGGING();
    ijk::io_t io;
    asio::ip::tcp::endpoint ep(asio::ip::address_v4::loopback(), 4000);

    auto connector = tcp_connector::create(io);
    connector
        ->on_end_connecting([](auto &ec, auto &&conn) {
            if (ec) {
                LOG_ERROR("failed to connect to server, error: {}", ec);
                return;
            }
            LOG_INFO("connected to server");
            conn->on_read([](auto &c, auto &data) { return data.size(); });
            conn->start();
            conn->send("111");
            conn->shutdown();
        })
        .retry_interval(std::chrono::milliseconds(3000))
        .connect(ep);

    /*
    auto client = tcp_client::create(io);
    client
        ->on_connected([](auto &c) {
            LOG_INFO("client is connected");
            c->send("111111");
        })
        .on_disconnected([](auto &, auto &ec) {
            LOG_INFO("cient disconnected with error: {}", ec);
        })
        .on_read([](auto &c, auto &data) {
            c->send(data);
            return data.size();
        })
        .connect_timeout(std::chrono::milliseconds(5000))
        //.reconnect_interval(std::chrono::milliseconds(3000))
        .connect(ep);
    */

    io.run();

    return 0;
}
