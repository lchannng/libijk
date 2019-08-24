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
#include "ijk/network/tcp_connector.hpp"

#include <map>
#include <memory>

namespace xx {

class client_service final : public network_service {
    struct server_connector final {
    public:
        using ptr = std::unique_ptr<server_connector>;
        server_connector(const server_addr &addr,
                         const asio::ip::tcp::endpoint &ep)
            : target_addr(addr), endpoint(ep) {}
        server_addr target_addr;
        asio::ip::tcp::endpoint endpoint;
        ijk::tcp_connector::ptr connector;
        ijk::tcp_connection::ptr conn;
    };

public:
    using ptr = std::unique_ptr<client_service>;

    size_t poll() { return io_.poll(); }

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

private:
    void connect_server(server_connector *s) {
        if (s->connector == nullptr)
            s->connector = ijk::tcp_connector::create(io_);

        s->connector
            ->on_end_connecting([this, s](auto &ec, auto &&conn) {
                if (nullptr == s) {
                    LOG_ERROR("failed to get server connector {}",
                              s->target_addr.to_string());
                    return;
                }

                if (ec) {
                    LOG_ERROR("failed to connect to server: {}, ep: {}",
                              s->target_addr.to_string(), s->endpoint);
                    return;
                }

                on_connected_to_server(s, std::move(conn));
            })
            .connect(s->endpoint);

        LOG_INFO("connecting to server: {}, ep: {}", s->target_addr.to_string(), s->endpoint);
    }

    void on_connected_to_server(server_connector *s,
                                ijk::tcp_connection::ptr &&conn) {
        s->conn = std::move(conn);
        s->conn
            ->on_closed([this, s](auto &conn, auto &ec) {
                Expects(s->conn.get() == conn.get());
                LOG_ERROR("lost connection to server: {}, ep: {}",
                          s->target_addr.to_string(), s->endpoint);
                s->conn.reset();

                // reconnect to server;
                connect_server(s);
            })
            .on_read([this, s](auto &conn, auto &data) { return data.size(); })
            .start();

        LOG_INFO("connected to server: {}, ep: {}", s->target_addr.to_string(), s->endpoint);
    }

    server_connector *get_server_connector(const server_addr &target_addr) {}

private:
    ijk::io_t io_;
    std::map<server_addr, server_connector::ptr> target_servers_;
};
}  // namespace xx

#endif /* end of include guard: CLIENT_SERVICE_HPP_IG32MN5Z */
