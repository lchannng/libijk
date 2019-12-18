/*
 * File  : future_server.cc
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/09/03 19:37:14
 */

#include "ijk/base/ignore_unused.hpp"
#include "ijk/base/logging.hpp"
#include "ijk/network/io.hpp"
#include "ijk/network/stream_connection.hpp"

using namespace ijk;

void handle_connection(const stream_connection::ptr &conn) {
    conn->on_message([](auto &s, auto &data) {
            s->send(data);
            // s->shutdown();
            return data.size();
        })
        .run();
}

void acceptor_loop(asio::ip::tcp::acceptor &acceptor, io_context_pool &pool) {
    auto &io = pool.get();
    ijk::accept<stream_connection>(acceptor, io)
        .then([](stream_connection::ptr conn) { handle_connection(conn); })
        .finally([&acceptor, &pool](auto e) {
            if (e.has_value()) {
                acceptor_loop(acceptor, pool);
            } else {
                try {
                    std::rethrow_exception(e.error());
                } catch (std::exception &ex) {
                    LOG_ERROR("acceptor error: {}", ex.what());
                } catch (asio::error_code &ec) {
                    LOG_ERROR("acceptor error: {}", ec);
                } catch (...) {
                    LOG_ERROR("acceptor error: unkonwn exception");
                }
            }
        });
}

int main(int argc, char *argv[]) {
    ignore_unused(argc, argv);
    IJK_INITIALIZE_LOGGING();

    ijk::io_context_pool pool;
    asio::ip::tcp::endpoint ep(asio::ip::address_v4::loopback(), 4000);
    asio::ip::tcp::acceptor acceptor(pool.get().context(), ep, true);
    acceptor_loop(acceptor, pool);
    pool.run();
    return 0;
}
