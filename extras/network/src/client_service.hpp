/*
 * File  : client_service.hpp
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/08/21 11:39:17
 */

#pragma once
#ifndef CLIENT_SERVICE_HPP_IG32MN5Z
#define CLIENT_SERVICE_HPP_IG32MN5Z

#include "network_service.hpp"

#include "ijk/network/asio_headers.hpp"

#include <memory>

namespace xx {
class client_service final : public network_service {
public:
    using ptr = std::unique_ptr<client_service>;

    size_t poll() { return 0; }

    void start_client(const server_addr &target_addr,
                      const asio::ip::tcp::endpoint &ep) {}

private:
};
}

#endif /* end of include guard: CLIENT_SERVICE_HPP_IG32MN5Z */
