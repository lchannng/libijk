/*
 * File  : tcp_service.h
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/07/07 22:22:52
 */

#pragma once
#ifndef TCP_SERVICE_H_PZITAISO
#define TCP_SERVICE_H_PZITAISO

#include "asio.h"

#include "ijk/base/noncopyable.h"
#include "ijk/base/string_view.h"

namespace ijk {

using SessionID = int64_t;

class TcpService final : public Noncopyable {
public:
    TcpService() = default;
    ~TcpService() = default;
    void runForever();

    void shutdown(SessionID session_id);
    void send(SessionID session_id, const string_view &data);

private:
    asio::io_context io_ctx_;
};
}

#endif /* end of include guard: TCP_SERVICE_H_PZITAISO */
