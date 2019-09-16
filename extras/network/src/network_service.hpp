/*
 * File  : network_service.hpp
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/08/21 11:40:00
 */

#pragma once
#ifndef NETWORK_SERVICE_HPP_T4QDJUSG
#define NETWORK_SERVICE_HPP_T4QDJUSG

#include "ijk/base/buffer.h"
#include "ijk/base/endian.h"
#include "ijk/network/base_connection.hpp"
#include "ijk/network/io.hpp"

namespace xx {

class server_connection final : public ijk::base_connection<server_connection> {
public:
    using ijk::base_connection<server_connection>::base_connection;
    ~server_connection() = default;
    void run() override {
        auto self = shared_from_this();
        ijk::read_exactly(socket(), asio::buffer(&head_, sizeof(head_)))
            .then([this, self](size_t bytes) {
                assert(bytes == sizeof(head_));
                head_ = betoh(head_);
                buf_.reserve(head_);
                return ijk::read_exactly(
                    socket(), asio::buffer(buf_.writable_head(), head_));
            })
            .then([this, self](auto bytes) {
                assert(bytes == head_);
                buf_.commit(bytes);
                invoke_message_cb(self,
                                  std::string_view(buf_.data(), buf_.size()));
                buf_.consume(bytes);
            })
            .finally([this, self](auto e) {
                if (!e.has_value()) {
                    invoke_close_cb(self, asio::error_code{});
                }
            });
    }

private:
    uint16_t head_;
    ijk::buffer buf_;
};

class network_service {
public:
    network_service() = default;
    virtual ~network_service() = default;
};
}

#endif /* end of include guard: NETWORK_SERVICE_HPP_T4QDJUSG */
