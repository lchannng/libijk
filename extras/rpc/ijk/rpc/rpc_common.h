/*
 * File  : rpc_common.h
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2019/07/06 19:56:43
 */

#pragma once

#ifndef RPC_COMMON_H
#define RPC_COMMON_H

#include "rpc_meta.pb.h"

#include <functional>

namespace ijk {

using RpcServiceID = decltype(std::declval<RpcMeta>().service_id());

using RpcMethodID = decltype(std::declval<RpcMeta>().request_info().intmethod());

using UnaryHandler =
    std::function<void(RpcMeta&&, const google::protobuf::Message&)>;

using UnaryServerInterceptor = std::function<void(
    RpcMeta&&, const google::protobuf::Message&, UnaryHandler&& next)>;

using RpcError = std::error_code;
}

#endif
