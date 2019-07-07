/*
 * File  : rcp_service.cc
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2019/07/06 19:27:52
 */

#include "rpc_service.h"

namespace ijk {

const UnaryServerInterceptor &RpcServiceContext::out_interceptor() const {
    return out_;
}

const UnaryServerInterceptor &RpcServiceContext::in_interceptor() const {
    return in_;
}

const RpcManager::Ptr &RpcServiceContext::rpc_manager() const {
    return rpc_manager_;
}

RpcServiceContext::RpcServiceContext(const RpcManager::Ptr &rpc_manager,
                                     UnaryServerInterceptor &&in,
                                     UnaryServerInterceptor &&out)
    : rpc_manager_(rpc_manager),
      in_(std::forward<UnaryServerInterceptor>(in)),
      out_(std::forward<UnaryServerInterceptor>(out)) {}

RpcService::RpcService(RpcServiceContext &&ctx)
    : ctx_(std::forward<RpcServiceContext>(ctx)) {}

const ijk::RpcServiceContext &RpcService::context() const {
    return ctx_;
}

}

