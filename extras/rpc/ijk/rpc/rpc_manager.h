/*
 * File  : rpc_manager.h
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2019/07/06 19:12:05
 */

#pragma once

#ifndef RPC_MANAGER_H
#define RPC_MANAGER_H

#include "rpc_common.h"

#include "ijk/base/noncopyable.h"
#include "ijk/base/string_view.h"

#include <cstdint>
#include <functional>
#include <map>
#include <mutex>

namespace ijk {

class RpcManager final : private Noncopyable {
public:
    using Ptr = std::shared_ptr<RpcManager>;
    RpcManager() = default;
    ~RpcManager() = default;

    using ServiceHandler = std::function<void(
        RpcMeta&& meta, const string_view& body, InterceptorContextType&&)>;

    bool registerHandler(RpcMeta::Type type, RpcServiceID service_id,
                         ServiceHandler&& handler);

    void removeHandler(RpcMeta::Type type, RpcServiceID service_id);

    void handleRpcMessage(RpcMeta&& meta, const string_view& body,
                          InterceptorContextType&&);

private:
    using HandlerMap = std::unordered_map<RpcServiceID, ServiceHandler>;
    std::mutex m_;
    std::map<RpcMeta::Type, HandlerMap> handlers_;
};

}

#endif