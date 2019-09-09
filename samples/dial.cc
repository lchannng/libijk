/*
 * File Name: future.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2019/08/30 13:56:08
 */

#include "ijk/base/ignore_unused.hpp"
#include "ijk/base/logging.hpp"
#include "ijk/network/io.hpp"
#include "ijk/network/tcp_connection.hpp"

using namespace ijk;

int main(int argc, char *argv[]) {
    ignore_unused(argc, argv);
    IJK_INITIALIZE_LOGGING();

    io_t io;

#if 0
    asio::ip::tcp::endpoint ep(asio::ip::make_address("127.0.0.1"), 4000);
    ijk::dial<tcp_connection>(io, ep)
        .then([](tcp_connection::ptr conn) {
            LOG_INFO("connected to server");
            conn->send(std::string("1111"));
        })
        .finally([](auto res) {
            if (!res.has_value()) {
                try {
                    std::rethrow_exception(res.error());
                } catch (const asio::system_error &e) {
                    LOG_ERROR("connect failed with error: {}", e.code());
                } catch (const asio::error_code &ec) {
                    LOG_ERROR("connect failed with error: {}", ec);
                }
            }
        });
#else
    ijk::dial<tcp_connection>(io, "localhost", 4000)
        .then([](auto conn) {
            LOG_INFO("connected to server");
            // conn->send(std::string("1111"));
        })
        .finally([](auto res) {
            if (!res.has_value()) {
                try {
                    std::rethrow_exception(res.error());
                } catch (const asio::system_error &e) {
                    LOG_ERROR("connect failed with error: {}", e.code());
                } catch (const asio::error_code &ec) {
                    LOG_ERROR("connect failed with error: {}", ec);
                }
            }
        });
#endif

    io.run();

    return 0;
}
