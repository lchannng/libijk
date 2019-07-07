/*
 * File  : rpc_manager.cc
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2019/07/06 19:11:59
 */

#include "rpc_manager.h"

namespace ijk {


bool RpcManager::registerHandler(RpcMeta::Type type, RpcServiceID service_id,
                                 ServiceHandler&& handler) {
    std::lock_guard<std::mutex> lock(m_);
    auto& map = handlers_[type];
    if (map.find(service_id) == map.end()) return false;
    map[service_id] = std::forward<ServiceHandler>(handler);
    return true;
}

void RpcManager::removeHandler(RpcMeta::Type type, RpcServiceID service_id) {
    std::lock_guard<std::mutex> lock(m_);
    auto& map = handlers_[type];
    if (map.find(service_id) == map.end()) return;
    map.erase(service_id);
}

void RpcManager::handleRpcMessage(RpcMeta&& meta, const string_view& body,
                                  InterceptorContextType &&ctx) {
    ServiceHandler handler;
    {
        std::lock_guard<std::mutex> lock(m_);
        auto it = handlers_.find(meta.type());
        if (it == handlers_.end()) return;
        auto& map = it->second;
        auto iter = map.find(meta.service_id());
        if (iter == map.end()) return;
        handler = iter->second;
    }
    handler(std::forward<RpcMeta>(meta), body, std::forward<InterceptorContextType>(ctx));
}
}
