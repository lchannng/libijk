/*
 * File  : future_server.cc
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/09/03 19:37:14
 */

#include "ijk/base/logging.hpp"
#include "ijk/network/base_connection.hpp"
#include "ijk/network/io.hpp"

#include <atomic>

using namespace ijk;

class echo_connection : public base_connection {
public:
    using ijk::base_connection::base_connection;

    virtual void run() override {
        if (is_closing_or_closed()) return;
        auto self = shared_from_this();
        buf_.reserve(1024);
        ijk::read_some(socket(), asio::buffer(buf_.writable_head(),
                                              buf_.writeable_bytes()))
            .then([this, self](auto bytes) {
                buf_.commit(bytes);
                size_t n = invoke_message_cb(self, std::string_view(buf_.data(), buf_.size()));
                buf_.consume(n);
            })
            .finally([this, self](auto e) {
                if (e.has_value()) {
                    run();
                } else {
                    invoke_close_cb(self, get_io_error_code(e.error()));
                }
            });
    }

private:
    buffer buf_;
};

void handle_connection(const echo_connection::ptr &conn) {
    conn->on_message([](auto &s, auto &data) {
            s->send(std::string(data));
            // s->shutdown();
            return data.size();
        })
        .on_close([](auto &s, auto &ec) {
            LOG_INFO("session {} closed: {}", s->id(), ec);
        })
        .run();
}

void acceptor_loop(asio::ip::tcp::acceptor &acceptor, io_context_pool &pool) {
    auto &io = pool.get();
    ijk::accept<echo_connection>(acceptor, io)
        .then([](auto conn) { handle_connection(conn); })
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
    IJK_INITIALIZE_LOGGING();

    ijk::io_context_pool pool;
    asio::ip::tcp::endpoint ep(asio::ip::address_v4::loopback(), 4000);
    asio::ip::tcp::acceptor acceptor(pool.get().context(), ep, true);
    acceptor_loop(acceptor, pool);
    pool.run();
    return 0;
}
