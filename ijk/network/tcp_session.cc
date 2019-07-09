/*
 * File  : tcp_session.cc
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/07/09 19:23:19
 */

#include "tcp_session.h"

namespace ijk {
std::atomic_uint32_t TcpSession::next_session_id_{0};

void TcpSession::read(asio::mutable_buffer buf, ReadHandler &&h) {
    asio::async_read(
        socket(), buf,
        [this, self = shared_from_this(), h = std::forward<ReadHandler>(h)](
            auto &ec, size_t bytes_transfered) { h((ec, bytes_transfered)); });
}

void TcpSession::readSome(asio::mutable_buffer buf, ReadHandler &&h) {
    socket_.async_read_some(
        buf,
        [this, self = shared_from_this(), h = std::forward<ReadHandler>(h)](
            auto &ec, size_t bytes_transfered) { h((ec, bytes_transfered)); });
}

}
