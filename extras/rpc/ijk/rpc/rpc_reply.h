/*
 * File  : rpc_reply.h
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2019/07/07 11:07:52
 */

#pragma once
#ifndef RPC_REPLY_H_BBQJ4WRD
#define RPC_REPLY_H_BBQJ4WRD

#include "rpc_common.h"

#include <atomic>
#include <string>
#include <unordered_map>
#include <functional>
#include <memory>

namespace ijk {

    class RpcReply
    {
    public:
        RpcReply(RpcMeta&& meta, UnaryServerInterceptor&& outboundInterceptor)
            :
            mRequestMeta(std::forward<RpcMeta>(meta)),
            mOutboundInterceptor(std::forward<UnaryServerInterceptor>(outboundInterceptor))
        {
        }

        virtual ~RpcReply() = default;

    protected:
        void    reply(const google::protobuf::Message& response, InterceptorContextType&& context)
        {
            if (mReplyFlag.test_and_set())
            {
                throw std::runtime_error("already reply");
            }

            if (!mRequestMeta.request_info().expect_response())
            {
                return;
            }

            RpcMeta meta;
            meta.set_type(RpcMeta::RESPONSE);
            meta.set_service_id(mRequestMeta.service_id());
            meta.mutable_response_info()->set_sequence_id(mRequestMeta.request_info().sequence_id());
            meta.mutable_response_info()->set_failed(false);
            meta.mutable_response_info()->set_timeout(false);

            mOutboundInterceptor(std::move(meta), 
                response, 
                [](RpcMeta&&, const google::protobuf::Message&, InterceptorContextType&& context) {
                }, 
                std::forward<InterceptorContextType>(context));
        }

        template<typename Response>
        void    error(int32_t errorCode, const std::string& reason, InterceptorContextType&& context)
        {
            if (mReplyFlag.test_and_set())
            {
                throw std::runtime_error("already reply");
            }

            if (!mRequestMeta.request_info().expect_response())
            {
                return;
            }

            RpcMeta meta;
            meta.set_type(RpcMeta::RESPONSE);
            meta.set_service_id(mRequestMeta.service_id());
            meta.mutable_response_info()->set_sequence_id(mRequestMeta.request_info().sequence_id());
            meta.mutable_response_info()->set_failed(true);
            meta.mutable_response_info()->set_error_code(errorCode);
            meta.mutable_response_info()->set_reason(reason);
            meta.mutable_response_info()->set_timeout(false);

            Response response;
            mOutboundInterceptor(meta, 
                response, 
                [](RpcMeta&&, const google::protobuf::Message&, InterceptorContextType&& context) {
                }, 
                std::forward<InterceptorContextType>(context));
        }

    private:
        std::atomic_flag            mReplyFlag = ATOMIC_FLAG_INIT;
        RpcMeta                     mRequestMeta;
        UnaryServerInterceptor      mOutboundInterceptor;
    };

    template<typename T>
    class RpcReplyT : public RpcReply
    {
    public:
        using Ptr = std::shared_ptr<RpcReplyT<T>>;

        RpcReplyT(RpcMeta&& meta,
            UnaryServerInterceptor&& outboundInterceptor)
            :
            RpcReply(std::move(meta), std::move(outboundInterceptor))
        {
        }

        void    reply(const T& response, InterceptorContextType&& context = InterceptorContextType())
        {
            RpcReply::reply(response, std::forward<InterceptorContextType>(context));
        }

        void    error(int32_t errorCode, const std::string& reason, InterceptorContextType&& context = InterceptorContextType())
        {
            RpcReply::error<T>(errorCode, reason, std::forward<InterceptorContextType>(context));
        }
    };

}


#endif /* end of include guard: RPC_REPLY_H_BBQJ4WRD */
