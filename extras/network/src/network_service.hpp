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

#include "protocol.pb.h"

#include <limits>

namespace xx {

class server_connection final : public ijk::base_connection<server_connection> {
public:
    using ijk::base_connection<server_connection>::base_connection;
    ~server_connection() = default;
    void run() {
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
                } else {
                    run();
                }
            });
    }

    void send(int32_t cmd, const google::protobuf::Message& msg) {
        ijk::buffer buf;
        if (encode(cmd, msg, buf)) {
            base_connection::send(std::string_view(buf.data(), buf.size()));
        }
    }

    static bool encode(int32_t cmd, const google::protobuf::Message& msg,
        ijk::buffer& buf) {

        using head_type = decltype(head_);

        uint32_t len = sizeof(cmd) + msg.ByteSizeLong();
        if (len > std::numeric_limits<head_type>::max()) {
            LOG_WARN("invalid payload len {}", len);
            return false;
        }

        head_type payload_len = static_cast<head_type>(len);
        buf.reserve(sizeof(head_) + payload_len);

        payload_len = htobe(payload_len);
        buf.append(&payload_len, 0, 1);

        cmd = htobe(cmd);
        buf.append(&cmd, 0, 1);

        auto msg_len = payload_len - sizeof(cmd);
        msg.SerializeToArray(buf.writable_head(), msg_len);
        buf.commit(msg_len);

        return true;
    }

    static bool decode(const std::string_view& data, int32_t& cmd,
        std::string_view& message_body) {
        if (data.size() < sizeof(cmd))
            return false;
        cmd = *(int32_t *)data.data();
        cmd = betoh(cmd);
        message_body = std::string_view(data.data() + sizeof(cmd), data.size() - sizeof(cmd));
        return true;
    }

private:
    uint16_t head_;
    ijk::buffer buf_;
};

class network_service {
public:
    network_service() = default;
    virtual ~network_service() = default;
    size_t poll() { return io_.poll(); }

protected:
    ijk::io_t io_;
};
}

#endif /* end of include guard: NETWORK_SERVICE_HPP_T4QDJUSG */
