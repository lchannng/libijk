/*
 * File  : network_service.hpp
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/08/21 11:40:00
 */

#pragma once
#ifndef NETWORK_SERVICE_HPP_T4QDJUSG
#define NETWORK_SERVICE_HPP_T4QDJUSG

#include "server_addr.hpp"

#include "ijk/base/buffer.h"
#include "ijk/base/endian.h"
#include "ijk/base/noncopyable.h"
#include "ijk/network/base_connection.hpp"
#include "ijk/network/io.hpp"

#include "protocol.pb.h"

#include <limits>

namespace xx {

class server_connection final : public ijk::base_connection<server_connection> {
    using head_type = uint16_t;

public:
    using ijk::base_connection<server_connection>::base_connection;
    ~server_connection() = default;
    void run() {
        message_loop(shared_from_this());
    }

    void send(int32_t cmd, const google::protobuf::Message &msg) {
        SNMessage sm;
        sm.set_cmd(SNCmd::SN_CMD_DATA);
        auto data = sm.mutable_data();
        data->set_app_msg_cmd(cmd);
        msg.SerializeToString(data->mutable_app_msg_data());
        send_message(sm);
    }

    const server_addr &target_addr() { return target_addr_; }

private:
    static bool encode(const SNMessage &msg,
                       std::string &buf) {
        auto len = msg.ByteSizeLong();
        if (len > std::numeric_limits<head_type>::max()) {
            LOG_WARN("invalid payload len {}", len);
            return false;
        }

        head_type payload_len = static_cast<head_type>(len);
        payload_len = htobe(payload_len);
        buf.append((const char *)&payload_len, sizeof(payload_len));
        msg.AppendToString(&buf);
        return true;
    }

    void message_loop(const ptr &self) {
        if (is_closing_or_closed()) return;
        ijk::read_exactly(socket(), asio::buffer(&head_, sizeof(head_)))
            .then([this, self](size_t bytes) {
                assert(bytes == sizeof(head_));
                buf_.clear();
                head_ = betoh(head_);
                buf_.reserve(head_);
                return ijk::read_exactly(
                    socket(), asio::buffer(buf_.writable_head(), head_));
            })
            .then([this, self](auto bytes) {
                assert(bytes == head_);
                buf_.commit(bytes);
                SNMessage msg;
                msg.ParseFromArray(buf_.data(), static_cast<int>(buf_.size()));
                process(msg);
            }).finally([this, self](auto e){
                if (e.has_value()) {
                    message_loop(self);
                } else {
                    close();
                }
            });
    }

    void send_handshake() {
        SNMessage msg;
        msg.set_cmd(SNCmd::SN_CMD_HANDSHAKE);
        send_message(msg);
    }

    inline void send_message(const SNMessage& msg) {
        std::string buf;
        if (encode(msg, buf)) {
            base_connection::send(std::move(buf));
        }
    }

    void close() { invoke_close_cb(shared_from_this(), asio::error_code()); }

    void process(const SNMessage &msg) {
        switch (msg.cmd()) {
            case ::xx::SN_CMD_HANDSHAKE: {
            } break;
            case ::xx::SN_CMD_DATA:
                break;
            default:
                break;
        }
    }

private:
    head_type head_;
    ijk::buffer buf_;
    server_addr target_addr_;
};

class network_service_manager : private ijk::noncopyable {
public:
    network_service_manager() = default;

    virtual ~network_service_manager() = default;

    void add_connection(const server_addr& target_addr,
        const server_connection::ptr& conn) {

    }

    void remove_connection(const server_addr& target_addr,
        const server_connection::ptr& conn) {

    }

    const server_addr &svr_addr() { return my_svr_addr_; }

protected:
    server_addr my_svr_addr_;
};

class network_service : private ijk::noncopyable {
public:
    // network_service(network_service_manager &manager) : manager_(manager) {}
    network_service() = default;
    virtual ~network_service() = default;
    size_t poll() { return io_.poll(); }

protected:
    ijk::io_t io_;
    // network_service_manager &manager_;
};
}  // namespace xx

#endif /* end of include guard: NETWORK_SERVICE_HPP_T4QDJUSG */
