/*
 * File Name: sample01.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2018/10/28 15:45:08
 */

#include "ijk/base/logging.hpp"
#include "ijk/network/io.hpp"
#include "ijk/network/tcp_connection.hpp"

using namespace ijk;

int main(int argc, char *argv[]) {
    IJK_INITIALIZE_LOGGING();
    ijk::io_t io;
    asio::ip::tcp::endpoint ep(asio::ip::address_v4::loopback(), 4000);

    ijk::dial<tcp_connection>(io, ep)
        .then([](auto conn) {
            conn->on_read([](auto &c, auto &data) {
                c->shutdown();
                return data.size();
            });
            conn->start();
            conn->send("111");
        })
        .finally([](auto res) {
            if (!res.has_value()) {
                try {
                    std::rethrow_exception(res.error());
                } catch (const std::exception &e) {
                    LOG_ERROR("connect failed with error: {}", e.what());
                } catch (const asio::error_code &ec) {
                    LOG_ERROR("connect failed with error: {}", ec);
                }
            }
        });

    io.run();

    return 0;
}
