/*
 * File  : client_service.hpp
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/08/21 11:39:17
 */

#pragma once
#ifndef CLIENT_SERVICE_HPP_IG32MN5Z
#define CLIENT_SERVICE_HPP_IG32MN5Z

#include "network_service.hpp"
#include "server_addr.hpp"

#include "ijk/base/logging.hpp"
#include "ijk/network/io.hpp"

#include <map>
#include <memory>

namespace xx {

class client_service final {
    struct server_connector final {
    public:
        using ptr = std::unique_ptr<server_connector>;
        server_connector(const server_addr &addr,
                         const asio::ip::tcp::endpoint &ep)
            : target_addr(addr), endpoint(ep) {}
        server_addr target_addr;
        asio::ip::tcp::endpoint endpoint;
        server_connection::ptr conn;
    };

public:
    using ptr = std::unique_ptr<client_service>;

    client_service(network_service_manager &manager) : manager_(manager) {}

    ~client_service() = default;

    void start_client(const server_addr &target_addr,
                      const asio::ip::tcp::endpoint &ep) {
        if (target_servers_.find(target_addr) != target_servers_.end()) {
            LOG_WARN("duplicated server {}", target_addr.to_string());
            return;
        }

        auto s = std::make_unique<server_connector>(target_addr, ep);
        connect_server(s.get());
        target_servers_.emplace(target_addr, std::move(s));
    }

    size_t poll() { return io_.poll(); }

private:
    void connect_server(server_connector *s) {

        ijk::dial<server_connection>(io_, s->endpoint)
            .then([this, s](auto conn) {
                on_connected_to_server(s, std::move(conn));
            })
            .finally([this, s](auto e) {
                if (!e.has_value()) {
                    LOG_ERROR("failed to connect to server: {}, ep: {}",
                              s->target_addr.to_string(), s->endpoint);
                    reconnect(s);
                }
            });

        LOG_INFO("connecting to server: {}, ep: {}", s->target_addr.to_string(), s->endpoint);
    }

    void on_connected_to_server(server_connector *s,
                                server_connection::ptr &&conn) {
        s->conn = std::move(conn);
        s->conn
            ->on_close([this, s](auto &conn, auto &ec) {
                Expects(s->conn.get() == conn.get());
                LOG_ERROR("lost connection to server: {}, ep: {}",
                          s->target_addr.to_string(), s->endpoint);
                s->conn.reset();
                reconnect(s);
            })
            .run_on_client_side(manager_, s->target_addr);

        LOG_INFO("connected to server: {}, ep: {}", s->target_addr.to_string(), s->endpoint);
    }

    void reconnect(server_connector* s) {
        ijk::delay(io_, std::chrono::seconds(3)).then([this, s] {
            // reconnect to server;
            connect_server(s);
        });
    }

private:
    ijk::io_t io_;
    network_service_manager &manager_;
    std::map<server_addr, server_connector::ptr> target_servers_;
};
}  // namespace xx

#endif /* end of include guard: CLIENT_SERVICE_HPP_IG32MN5Z */
