/*
 * File  : rcp_client.h
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2019/07/06 19:27:40
 */

#pragma once

#ifndef RPC_CLIENT_H
#define RPC_CLIENT_H

#include "rpc_common.h"
#include "rpc_manager.h"

#include <memory>

namespace ijk {

class RpcClient : public std::enable_shared_from_this<RpcClient> {
public:
    template<typename Response, typename Request, typename Handler>
    void call(const Request& req, RpcServiceID service_id, RpcMethodID method, Handler&& handler) {
        auto seq = seq_++;
        RpcMeta meta;
        meta.set_type(RpcMeta::REQUEST);
        meta.set_service_id(service_id);
        auto req_info = meta.mutable_request_info();
        req_info->set_intmethod(method);
        req_info->set_sequence_id(seq);
        out_(std::move(meta), req,
             [](RpcMeta&&, const google::protobuf::Message&) {});
    }

    void install(const RpcManager::Ptr &rpc_manager, RpcServiceID service_id);

protected:
    RpcClient(const RpcManager::Ptr &rpc_manager, UnaryServerInterceptor &&in,
              UnaryServerInterceptor&& out);

    virtual ~RpcClient() = default;
    
protected:
    RpcManager::Ptr rpc_manager_;
    UnaryServerInterceptor in_;
    UnaryServerInterceptor out_;
    std::atomic_uint64_t seq_;
};

}

#endif
