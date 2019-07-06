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

}
