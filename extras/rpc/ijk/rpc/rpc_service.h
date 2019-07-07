/*
 * File  : rcp_service.h
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2019/07/06 19:28:02
 */

#pragma once

#ifndef _RPC_SERVICE_H
#define _RPC_SERVICE_H

#include "rpc_common.h"
#include "rpc_manager.h"

#include "ijk/base/noncopyable.h"

#include <memory>

namespace ijk {

class RpcServiceContext final {
public:
    RpcServiceContext(const RpcManager::Ptr& rpc_manager,
                      UnaryServerInterceptor&& in, UnaryServerInterceptor&& out);

    ~RpcServiceContext() = default;

    const RpcManager::Ptr& rpc_manager() const;

    const UnaryServerInterceptor& in_interceptor() const;

    const UnaryServerInterceptor& out_interceptor() const;

private:
    RpcManager::Ptr rpc_manager_;
    UnaryServerInterceptor in_;
    UnaryServerInterceptor out_;
};

class RpcService : public std::enable_shared_from_this<RpcService>, public Noncopyable {
public:
    RpcService(RpcServiceContext &&ctx);
    virtual ~RpcService() = default;
    const RpcServiceContext &context() const;

private:
    RpcServiceContext ctx_;
};

}

#endif
