/*
 * File Name: future.cc
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2019/08/30 13:56:08
 */

#include "ijk/base/ignore_unused.hpp"
#include "ijk/base/logging.hpp"
#include "ijk/future/future.h"

using namespace ijk;

int main(int argc, char *argv[]) {
    ignore_unused(argc, argv);
    IJK_INITIALIZE_LOGGING();

    {
        promise<int> pm;

        auto fut = pm.get_future();
        fut.then([](int n) {
            LOG_INFO("n: {}", n);
            throw std::logic_error("error");
        }).finally([](auto n) {
            LOG_INFO("finally");
        });

        pm.set_value(111);
        // pm.set_exception(std::make_exception_ptr(std::logic_error("error")));
    }

    {
        promise<void> pm;
        pm.set_value();

        auto fut = pm.get_future();
        fut.finally([](auto) {
            LOG_INFO("finally");
        });
    }

    { 
        auto fut = make_ready_future(1);
        fut.finally([](auto n){
            LOG_INFO("n: {}", *n);
        });
    }

    { 
        auto fut = make_ready_future(1, 3);
        fut.finally([](auto n0, auto n1){
            LOG_INFO("n0: {}, n1: {}", *n0, *n1);
        });
    }

    {
        auto fut = make_ready_future();
        fut.finally([](auto){
            LOG_INFO("finally");
        });
    }

    { 
        auto fut = make_exception_future<int>(std::runtime_error("error"));
        fut.finally([](auto n){
            LOG_INFO("finally");
        });
    }

    {
        auto fut = make_ready_future(1);
        fut.then([](int n) {
               if (n == 1) {
                   return make_exception_future<int>(
                       std::runtime_error("error"));
               }
               return make_ready_future(n * 2);
           })
            .finally([](auto n) {
                LOG_INFO("finally");
            });
    }

    return 0;
}
