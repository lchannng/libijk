/*
 * File  : server_network.hpp
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/08/20 19:25:17
 */

#pragma once
#ifndef SERVER_NETWORK_HPP_G3OOXXHE
#define SERVER_NETWORK_HPP_G3OOXXHE

#include "server_addr.hpp"
#include "client_service.hpp"

#include "ijk/base/gsl.h"
#include "ijk/network/asio_compatibility.hpp"

#include <map>

namespace xx {

class server_network final {
public:
    server_network(const server_addr &my_svr_addr) : my_svr_addr_(my_svr_addr) {} 

    ~server_network() {}

    void start_client(const server_addr &target_addr, const asio::ip::tcp::endpoint &ep) {
        auto iter = client_services_.find(target_addr.type_id);
        if (iter == client_services_.end()) {
            auto res = client_services_.emplace(target_addr.type_id, std::make_unique<client_service>()); 
            Ensures(res.second);
            iter = res.first;
        }
        iter->second->start_client(target_addr, ep);
    }

    size_t poll() {
        size_t count = 0;
        for (auto& client : client_services_) {
            count += client.second->poll();
        }
        return count;
    }

private:
    server_addr my_svr_addr_;
    std::map<uint8_t, client_service::ptr> client_services_;
};

}

#endif /* end of include guard: SERVER_NETWORK_HPP_G3OOXXHE */
