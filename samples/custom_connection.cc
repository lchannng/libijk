/*
 * File  : future_server.cc
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/09/03 19:37:14
 */

#include "ijk/base/ignore_unused.hpp"
#include "ijk/base/logging.hpp"
#include "ijk/network/base_connection.hpp"
#include "ijk/network/io.hpp"

#include <array>

using namespace ijk;

class echo_connection : public base_connection<echo_connection> {
public:
    using base_connection::base_connection;

    virtual void run() override {
        if (is_closing_or_closed()) return;
        auto self = shared_from_this();
        ijk::read_some(socket(), asio::buffer(buf_.data(), buf_.size()))
            .then([this, self](auto ec, auto bytes) {
                if (ec) {
                    invoke_close_cb(self, ec);
                } else {
                    send(std::string(buf_.data(), bytes));
                    run();
                }
            });
    }

private:
    std::array<char, 1024> buf_;
};

void acceptor_loop(asio::ip::tcp::acceptor &acceptor, io_context_pool &pool) {
    auto &io = pool.get();
    ijk::accept<echo_connection>(acceptor, io)
        .then([](auto conn) { conn->run(); })
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
