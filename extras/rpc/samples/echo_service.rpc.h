#pragma once

#ifndef _ECHO_SERVICE_H
#define _ECHO_SERVICE_H

#include "echo_service.pb.h"

#include "ijk/rpc/rpc_client.h"

namespace ijk {
namespace test {

const uint32_t echo_service_id = 0;

enum echo_service_method {
    Echo = 0,
};

class EchoServiceClient : public ijk::RpcClient {
public:
    using Ptr = std::shared_ptr<EchoServiceClient>;
    using EchoHandler = std::function<void(const ijk::test::EchoResponse&,
                                           const ijk::RpcError&)>;

    void Echo(const ijk::test::EchoRequest&& req, EchoHandler&& handler) {
        call<EchoResponse>(req, echo_service_id, echo_service_method::Echo,
                           std::forward<EchoHandler>(handler));
    }

    static Ptr create(const RpcManager::Ptr& rpc_manager,
                      UnaryServerInterceptor&& in,
                      UnaryServerInterceptor&& out) {
        class MakeSharedProxy : public EchoServiceClient {
        public:
            MakeSharedProxy(const RpcManager::Ptr& rpc_manager,
                                UnaryServerInterceptor&& in,
                                UnaryServerInterceptor&& out)
                : EchoServiceClient(rpc_manager,
                                    std::forward<UnaryServerInterceptor>(in),
                                    std::forward<UnaryServerInterceptor>(out)) {
            }
        };

        auto client = std::make_shared<MakeSharedProxy>(
            rpc_manager, std::forward<UnaryServerInterceptor>(in),
            std::forward<UnaryServerInterceptor>(out));
        client->install(rpc_manager, echo_service_id);
        return client;
    }

private:
    using ijk::RpcClient::RpcClient;
};

}
}


#endif

