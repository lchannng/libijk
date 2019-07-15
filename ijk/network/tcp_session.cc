/*
 * File  : tcp_session.cc
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/07/09 19:23:19
 */

#include "tcp_session.h"

#include "ijk/base/gsl.h"
#include "ijk/log/log.h"

#include <cassert>

namespace ijk {
std::atomic_uint64_t TcpSession::next_session_id_{0};

TcpSession::TcpSession(io_t &io)
    : id_(++next_session_id_),
      io_(io),
      socket_(io_.context()),
      writing_(false) {}

TcpSession::~TcpSession() {
    LOG_DEBUG("session {} closed", id_);
}

std::string TcpSession::localAddress() {
    if (!socket_.is_open()) return std::string();
    auto ep = socket_.local_endpoint();
    return ep.address().to_string() + ":" + std::to_string(ep.port());
}

std::string TcpSession::remoteAddress() {
    if (!socket_.is_open()) return std::string();
    auto ep = socket_.remote_endpoint();
    return ep.address().to_string() + ":" + std::to_string(ep.port());
}

void TcpSession::start() {
    Expects(on_read_ != nullptr);
    if (io_.running_in_this_thread()) {
        postRead();
    } else {
        io_.post([this, self = shared_from_this()]() { postRead(); });
    }

}

void TcpSession::stop() {
    if (io_.running_in_this_thread()) {
        onClose(shared_from_this(), asio::error_code());
    } else {
        io_.post([this, self = shared_from_this()]() {
            onClose(self, asio::error_code());
        });
    }
}

void TcpSession::send(const string_view &data) {
    if (io_.running_in_this_thread()) {
        if (!socket_.is_open()) return;
        send_queue_.push_back(std::string(data.data(), data.size()));
        if (sending_queue_.empty());
            postWrite();
        return;
    }

    io_.post([this, self = shared_from_this(),
              d = std::string(data.data(), data.size())]() {
        if (!socket_.is_open()) return;
        send_queue_.push_back(std::move(d));
        if (sending_queue_.empty()) postWrite();
    });
}

TcpSession &TcpSession::onRead(ReadCallback &&cb) {
    // Expects(io_.running_in_this_thread());
    on_read_ = std::forward<ReadCallback>(cb);
    return *this;
}

TcpSession &TcpSession::onClosed(CloseCallback &&cb) {
    // Expects(io_.running_in_this_thread());
    on_closed_ = std::forward<CloseCallback>(cb);
    return *this;
}

void TcpSession::postRead() {
    assert(io_.running_in_this_thread());
    Expects(socket_.is_open());
    recv_buf_.reserve(4096);
    socket_.async_read_some(
        asio::buffer(recv_buf_.tail(), recv_buf_.tailRoom()),
        [this, self = shared_from_this()](auto &ec, auto bytes_transfered) {
            if (ec) {
                onClose(self, ec);
                return;
            }
            recv_buf_.commit(bytes_transfered);
            if (on_read_) {
                size_t nbytes = on_read_(
                    self,
                    string_view((char *)recv_buf_.data(), recv_buf_.size()));
                recv_buf_.drain(nbytes);
            }
            postRead();
        });
}

void TcpSession::postWrite() {
    assert(io_.running_in_this_thread());
    sending_buffers_.clear();
    sending_queue_.clear();
    if (send_queue_.empty()) return;
    if (!socket_.is_open()) return;
    sending_queue_.swap(send_queue_);
    for (const auto &data : sending_queue_) {
        sending_buffers_.emplace_back(asio::buffer(data.data(), data.size()));
    }

    asio::async_write(
        socket_, sending_buffers_,
        [this, self = shared_from_this()](auto &ec, auto bytes_transfered) {
            if (ec) {
                onClose(self, ec);
                return;
            }
            postWrite();
        });
}

void TcpSession::onClose(const Ptr &self, const asio::error_code &ec) {
    assert(io_.running_in_this_thread());
    if (on_closed_) on_closed_(self, ec);
    on_closed_ = nullptr;
    on_read_ = nullptr;
    if (socket_.is_open()) {
        asio::error_code ignored_ec;
        socket_.shutdown(asio::socket_base::shutdown_both, ignored_ec);
        socket_.close(ignored_ec);
    }
}

}
