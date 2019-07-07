/*
 * File  : main.cc
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2019/07/06 21:55:02
 */

 #include "echo_service.ijkrpc.h"

 using namespace ijk;
 using namespace ijk::test;

int main(int argc, char *argv[]) {
    auto rpc_manager = std::make_shared<RpcManager>();
    auto in = [](RpcMeta&&, const google::protobuf::Message&,
                 UnaryHandler&& next, InterceptorContextType &&) {};
    auto out = [](RpcMeta&&, const google::protobuf::Message&,
                  UnaryHandler&& next, InterceptorContextType &&) {};
    auto client = EchoClient::create(rpc_manager, std::move(in), std::move(out));
    return 0;
}
