/*
 * File  : main.cc
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/09/12 10:31:33
 */

#include "server_network.hpp"

int main(int argc, char *argv[])
{
    IJK_INITIALIZE_LOGGING();

    auto addr = asio::ip::make_address("127.0.0.1");
    asio::ip::tcp::endpoint ep(addr, 4000);

    xx::server_addr sa1(1, 1, 100, 1);
    xx::server_network sn1(sa1);
    sn1.start_server(ep);

    xx::server_addr sa2(1, 1, 200, 1);
    xx::server_network sn2(sa2);
    sn2.start_client(sa1, ep);

    while (true) {
        auto count = sn1.poll();
        count += sn2.poll();
        if (count == 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}
