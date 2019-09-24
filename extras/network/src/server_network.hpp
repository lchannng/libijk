/*
 * File  : server_network.hpp
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/08/20 19:25:17
 */

#pragma once
#ifndef SERVER_NETWORK_HPP_G3OOXXHE
#define SERVER_NETWORK_HPP_G3OOXXHE

#include "client_service.hpp"
#include "server_addr.hpp"
#include "server_service.hpp"

#include "ijk/base/gsl.h"

#include <map>

namespace xx {

class server_network final : public network_service_manager {
public:
    server_network(const server_addr &my_svr_addr)
        : network_service_manager(my_svr_addr) {} 

    ~server_network() {}

    void start_client(const server_addr &target_addr, const asio::ip::tcp::endpoint &ep) {
        auto iter = client_services_.find(target_addr.type_id);
        if (iter == client_services_.end()) {
            auto res = client_services_.emplace(target_addr.type_id, std::make_unique<client_service>(*this)); 
            Ensures(res.second);
            iter = res.first;
        }
        iter->second->start_client(target_addr, ep);
    }

    void start_server(const asio::ip::tcp::endpoint &ep) {
        Ensures(server_service_ == nullptr);
        server_service_ = std::make_unique<server_service>(*this, my_svr_addr_, ep);
        server_service_->start_server();
    }

    size_t poll() {
        size_t count = 0;
        count += server_service_->poll();
        for (auto& client : client_services_) {
            count += client.second->poll();
        }
        return count;
    }

private:
    std::map<uint8_t, client_service::ptr> client_services_;
    std::unique_ptr<server_service> server_service_;
};

}

#endif /* end of include guard: SERVER_NETWORK_HPP_G3OOXXHE */
