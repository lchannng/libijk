/*
 * File  : stream_connection.h
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/07/09 19:23:21
 */

#pragma once
#ifndef TCP_SESSION_H_2RZWVGWL
#define TCP_SESSION_H_2RZWVGWL

#include "io_context.hpp"

#include "ijk/base/buffer.h"
#include "ijk/base/gsl.h"
#include "ijk/base/logging.hpp"
#include "ijk/network/base_connection.hpp"

#include <cassert>
#include <list>
#include <string_view>

namespace ijk {

class stream_connection final : public base_connection<stream_connection> {
public:
    using message_callback =
        std::function<size_t(const ptr &, const std::string_view &)>;

    using base_connection::base_connection;

    stream_connection &on_message(message_callback &&cb) {
        // Expects(io_.running_in_this_thread());
        message_cb_ = std::forward<message_callback>(cb);
        return *this;
    }


    void run() {
        if (io().running_in_this_thread()) {
            read_loop();
        } else {
            io().post([this, self = shared_from_this()]() { read_loop(); });
        }
    }

private:
    void read_loop() {
        if (is_closing_or_closed()) return;
        auto self = shared_from_this();
        recv_buf_.reserve(4096);
        ijk::read_some(socket(), asio::buffer(recv_buf_.writable_head(),
                                              recv_buf_.writeable_bytes()))
            .finally([this, self](auto e) {
                if (!e.has_value()) {
                    invoke_close_cb(self, asio::error_code{});
                } else {
                    recv_buf_.commit(e.value());
                    size_t n = invoke_message_cb(
                        self,
                        std::string_view(recv_buf_.data(), recv_buf_.size()));
                    recv_buf_.consume(n);

                    read_loop();
                }
            });
    }

    inline size_t invoke_message_cb(const ptr &self,
                                    const std::string_view &buf) {
        if (message_cb_) return message_cb_(self, buf);
        return buf.size();
    }

    buffer recv_buf_;
    message_callback message_cb_;
};

}  // namespace ijk

#endif /* end of include guard: TCP_SESSION_H_2RZWVGWL */
