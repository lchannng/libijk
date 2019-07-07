/*
 * File  : rcp_client.cc
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2019/07/06 19:27:37
 */

#include "rpc_client.h"

namespace ijk {

RpcClient::RpcClient(const RpcManager::Ptr &rpc_manager,
                     UnaryServerInterceptor &&in, UnaryServerInterceptor &&out)
    : rpc_manager_(rpc_manager), in_(in), out_(out), seq_(0) {}

void RpcClient::install(const RpcManager::Ptr &rpc_manager,
                        RpcServiceID service_id) {
    auto self = shared_from_this();
    auto stub = [](RpcMeta &&meta, const string_view &data,
                   InterceptorContextType &&) {
        // TODO
    };
    rpc_manager->registerHandler(RpcMeta::RESPONSE, service_id, std::move(stub));
}

}  // namespace ijk
