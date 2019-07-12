// Generated by github.com/lchannng/protoc-gen-ijkrpc
// Coding by github.com/liuhan907
// DO NOT EDIT!!!

#ifndef IJK_TEST_ECHO_SERVICE_H
#define IJK_TEST_ECHO_SERVICE_H

#include "echo_service.pb.h"

#include "ijk/rpc/rpc_proxy.h"
#include "ijk/rpc/rpc_reply.h"
#include "ijk/rpc/rpc_service.h"

#include "ijk/base/string_view.h"

namespace ijk {
namespace test {


using namespace ijk;

enum class echo_service_ServiceID:uint32_t
{
    Echo,
    
};


enum class EchoMsgID:uint64_t
{
    Echo = 2333,
    
};

class EchoServiceProxy : public RpcProxy
{
public:
    using Ptr = std::shared_ptr<EchoServiceProxy>;
    using WeakPtr = std::weak_ptr<EchoServiceProxy>;

    using EchoHandler = std::function<void(const ijk::test::EchoResponse&, const RpcError&)>;
    

public:
    void Echo(const ijk::test::EchoRequest& request,
        const EchoHandler& handler = nullptr)
    {
        call<ijk::test::EchoResponse>(request, 
            static_cast<uint32_t>(echo_service_ServiceID::Echo), 
            static_cast<uint64_t>(EchoMsgID::Echo), 
            handler);
    }
    

    /*
    void Echo(const ijk::test::EchoRequest& request,
        const EchoHandler& handler,
        std::chrono::seconds timeout, 
        RpcProxy::TIMEOUT_CALLBACK&& timeoutCallback)
    {
        call<ijk::test::EchoResponse>(request, 
            static_cast<uint32_t>(echo_service_ServiceID::Echo), 
            static_cast<uint64_t>(EchoMsgID::Echo), 
            handler,
            timeout,
            std::move(timeoutCallback));
    }
    
    */

public:
    static Ptr create(const RpcManager::Ptr& rpc_manager,
        UnaryServerInterceptor &&in,
        UnaryServerInterceptor &&out)
    {
        class MakeSharedProxy : public EchoServiceProxy {
        public:
            MakeSharedProxy(const RpcManager::Ptr& rpc_manager,
                            UnaryServerInterceptor&& in,
                            UnaryServerInterceptor&& out)
                : EchoServiceProxy(rpc_manager,
                             std::forward<UnaryServerInterceptor>(in),
                             std::forward<UnaryServerInterceptor>(out)) {}
        };

        auto client = std::make_shared<MakeSharedProxy>(
            rpc_manager, std::forward<UnaryServerInterceptor>(in),
            std::forward<UnaryServerInterceptor>(out));
        client->install(rpc_manager, static_cast<uint32_t>(echo_service_ServiceID::Echo));

        return client;
    }

    static  std::string GetServiceTypeName()
    {
        return "ijk::test::Echo";
    }

private:
    using RpcProxy::RpcProxy;
};

class EchoService : public RpcService
{
public:
    using Ptr = std::shared_ptr<EchoService>;
    using WeakPtr = std::weak_ptr<EchoService>;

    using EchoReply = RpcReplyT<ijk::test::EchoResponse>;
    

    using RpcService::RpcService;

    virtual ~EchoService()
    {
    }

    virtual void onClose() {}

    static bool Install(const EchoService::Ptr& service);

    static  std::string GetServiceTypeName()
    {
        return "ijk::test::Echo";
    }
private:
    virtual void Echo(const ijk::test::EchoRequest& request, 
        const ijk::test::EchoService::EchoReply::Ptr& replyObj,
        InterceptorContextType&&) = 0;
    

private:

    static void Echo_stub(RpcMeta&& meta,
        const string_view& data,
        const EchoService::Ptr& service,
        const UnaryServerInterceptor& in,
        const UnaryServerInterceptor& out,
        InterceptorContextType&& context)
    {
        ijk::test::EchoRequest request;
        parseRequestWrapper(request, std::move(meta), data, in, [service,
            out = out,
            &request](RpcMeta&& meta, const google::protobuf::Message& message, InterceptorContextType&& context) mutable {
            auto replyObject = std::make_shared<EchoReply>(std::move(meta), std::move(out));
            service->Echo(request, replyObject, std::move(context));
        }, std::move(context));
    }
    
};

inline bool EchoService::Install(const EchoService::Ptr& service)
{
    auto rpc_manager = service->context().rpc_manager();
    auto in = service->context().in_interceptor();
    auto out = service->context().out_interceptor();

    using EchoServiceRequestHandler = std::function<void(RpcMeta&&,
        const string_view& data,
        const EchoService::Ptr&,
        const UnaryServerInterceptor&,
        const UnaryServerInterceptor&,
        InterceptorContextType&& context)>;

    using EchoServiceHandlerMapById = std::unordered_map<uint64_t, EchoServiceRequestHandler>;
    using EchoServiceHandlerMapByStr = std::unordered_map<std::string, EchoServiceRequestHandler>;

    // TODO::static unordered map
    auto serviceHandlerMapById = std::make_shared<EchoServiceHandlerMapById>();
    auto serviceHandlerMapByStr = std::make_shared<EchoServiceHandlerMapByStr>();

    const std::string namespaceStr = "ijk.test.";

    (*serviceHandlerMapById)[static_cast<uint64_t>(EchoMsgID::Echo)] = EchoService::Echo_stub;
    
    (*serviceHandlerMapByStr)[namespaceStr+"Echo.Echo"] = EchoService::Echo_stub;
    

    auto requestStub = [service,
        serviceHandlerMapById,
        serviceHandlerMapByStr,
        in,
        out](RpcMeta&& meta, const string_view& data, InterceptorContextType&& context) {
        
        if (meta.type() != RpcMeta::REQUEST)
        {
            throw std::runtime_error("meta type not request, It is:" + std::to_string(meta.type()));
        }
        
        EchoServiceRequestHandler handler;

        if (!meta.request_info().method_name().empty())
        {
            auto it = serviceHandlerMapByStr->find(meta.request_info().method_name());
            if (it == serviceHandlerMapByStr->end())
            {
                throw std::runtime_error("not found handle, method:" + meta.request_info().method_name());
            }
            handler = (*it).second;
        }
        else
        {
            auto it = serviceHandlerMapById->find(meta.request_info().method_id());
            if (it == serviceHandlerMapById->end())
            {
                throw std::runtime_error("not found handle, method:" + meta.request_info().method_id());
            }
            handler = (*it).second;
        }

        handler(std::move(meta),
            data,
            service,
            in,
            out,
            std::move(context));
    };

    return rpc_manager->registerHandler(RpcMeta::REQUEST, static_cast<uint32_t>(echo_service_ServiceID::Echo), requestStub);
}

}
}

#endif

