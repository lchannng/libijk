/*
 * File  : main.cc
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2019/07/06 21:55:02
 */

 #include "echo_service.ijkrpc.h"

 #include "ijk/log/logger.h"

 using namespace ijk;
 using namespace ijk::test;

 class EchoServiceImpl : public EchoService {
 public:
     EchoServiceImpl(RpcServiceContext &&ctx)
         : EchoService(std::forward<RpcServiceContext>(ctx)) {}
     void Echo(const ijk::test::EchoRequest &request,
               const ijk::test::EchoService::EchoReply::Ptr &replyObj,
               InterceptorContextType &&ctx) override {
         LOG_INFO("echo service");
         EchoResponse rsp;
         rsp.set_message("pong");
         replyObj->reply(rsp, std::forward<InterceptorContextType>(ctx));
     }
 };

int main(int argc, char *argv[]) {
    IJK_INITIALIZE_LOGGING();

    auto rpc_manager = std::make_shared<RpcManager>();


    auto service_in = [](RpcMeta &&meta, const google::protobuf::Message &msg,
                         UnaryHandler &&next, InterceptorContextType &&ctx) {
        LOG_INFO("service in interceptor");
        next(std::forward<RpcMeta>(meta), msg,
             std::forward<InterceptorContextType>(ctx));
    };

    auto service_out = [](RpcMeta &&, const google::protobuf::Message &,
                  UnaryHandler &&next,
                  InterceptorContextType &&) { LOG_INFO("service out interceptor"); };

    RpcServiceContext ctx(rpc_manager, service_in, service_out);

    auto service = std::make_shared<EchoServiceImpl>(std::move(ctx));
    service->Install(service);

    auto in = [](RpcMeta &&, const google::protobuf::Message &,
                 UnaryHandler &&next,
                 InterceptorContextType &&) { LOG_INFO("in interceptor"); };
    auto out = [=](RpcMeta &&meta, const google::protobuf::Message &msg,
                   UnaryHandler &&next, InterceptorContextType &&) {
        LOG_INFO("out interceptor");
        std::string data = msg.SerializeAsString();
        rpc_manager->handleRpcMessage(std::forward<RpcMeta>(meta), data,
                                      InterceptorContextType{});
    };
    auto proxy = EchoServiceProxy::create(rpc_manager, std::move(in), std::move(out));

    EchoRequest req;
    req.set_message("ping");
    proxy->Echo(req, [](auto rsp, auto err) {
    
    });

    return 0;
}
