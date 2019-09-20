/*
 * File  : server_service.hpp
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/09/17 20:05:11
 */
#pragma once
#ifndef SERVER_SERVICE_HPP_RUMNQWKF
#define SERVER_SERVICE_HPP_RUMNQWKF

#include "network_service.hpp"
#include "server_addr.hpp"

#include "ijk/base/logging.hpp"
#include "ijk/network/io.hpp"

namespace xx {
class server_service final : public network_service {
public:
    server_service(const server_addr &addr, const asio::ip::tcp::endpoint &ep)
        : saddr_(addr), acceptor_(io_.context(), ep, true) {}

    ~server_service() = default;

    void start_server() {
        acceptor_loop();
    }

private:
    void acceptor_loop() {
        if (!acceptor_.is_open())
            return;

        if (!next_conn_) next_conn_ = std::make_shared<server_connection>(io_);

        if (next_conn_->socket().is_open()) {
            asio::error_code ec;
            next_conn_->socket().close(ec);
        }

        ijk::accept(acceptor_, next_conn_->socket())
            .then([this]() { handle_connection(std::move(next_conn_)); })
            .finally([this](auto e) {
                if (e.has_value()) {
                    acceptor_loop();
                } else {
                    LOG_ERROR("accept error, retry in 3s.");
                    ijk::delay(io_, std::chrono::seconds(3)).then([this]() {
                        acceptor_loop();
                    });
                }
            });
    }

    void handle_connection(server_connection::ptr conn) {
        LOG_INFO("accept connection from {}", conn->remote_address());
    } 

private:
    server_addr saddr_;
    asio::ip::tcp::acceptor acceptor_;
    server_connection::ptr next_conn_;
};
}

#endif /* end of include guard: SERVER_SERVICE_HPP_RUMNQWKF */
