/*
 * File  : main.cc
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/09/12 10:31:33
 */

#include "server_network.hpp"

int main(int argc, char *argv[])
{
    IJK_INITIALIZE_LOGGING();
    xx::server_network sn(xx::server_addr(1, 1, 100, 1));

    auto addr = asio::ip::make_address("127.0.0.1");
    asio::ip::tcp::endpoint ep(addr, 4000);
    sn.start_client(xx::server_addr(1, 1, 101, 1), ep);

    while (true) {
        auto count = sn.poll();
        if (count == 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}
