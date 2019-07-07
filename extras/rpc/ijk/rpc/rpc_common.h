/*
 * File  : rpc_common.h
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2019/07/06 19:56:43
 */

#pragma once

#ifndef RPC_COMMON_H
#define RPC_COMMON_H

#include "rpc_meta.pb.h"

#include "ijk/base/string_view.h"

#include <functional>

namespace ijk {

struct InterceptorContextType {};

using RpcServiceID = decltype(std::declval<RpcMeta>().service_id());

using RpcMethodID =
    decltype(std::declval<RpcMeta>().request_info().intmethod());

using UnaryHandler = std::function<void(
    RpcMeta&&, const google::protobuf::Message&, InterceptorContextType &&)>;

using UnaryServerInterceptor = std::function<void(RpcMeta&&, const google::protobuf::Message&,
                       UnaryHandler&& next, InterceptorContextType &&context)>;

using RpcError = std::error_code;

// 解析Request然后(通过拦截器)调用服务处理函数
template <typename RequestType, typename UnaryServerInterceptor>
inline void parseRequestWrapper(
    RequestType& request, RpcMeta&& meta, const string_view& data,
    const UnaryServerInterceptor& inboundInterceptor, UnaryHandler&& handler,
    InterceptorContextType&& context) {

    if (!request.ParseFromArray(data.data(), data.size())) {
        throw std::runtime_error(
            std::string("parse binary request failed, type of:") +
            typeid(RequestType).name());
    }

    inboundInterceptor(std::move(meta), request, std::move(handler),
                       std::forward<InterceptorContextType>(context));
}

}  // namespace ijk

#endif
