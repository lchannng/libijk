/*
 * File  : benchmark.cc
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/08/07 19:40:12
 */

#include "ijk/base/ignore_unused.hpp"
#include "ijk/base/logging.hpp"
#include "ijk/base/mpmc_blocking_q.hpp"
#include "ijk/base/mpmc_queue.h"
#include "ijk/base/random.h"
#include "ijk/base/stopwatch.h"
#include "ijk/network/io_context.hpp"

#include <any>
#include <iostream>
#include <mutex>
#include <shared_mutex>

class base : public std::enable_shared_from_this<base> {
public:
    base() = default;
    int a {0};
    virtual ~base() = default;
};

class derived : public base {
public:
    derived() = default;
    ~derived() = default;
    int b {0};
};

int main(int argc, char *argv[])
{
    ijk::ignore_unused(argc, argv);
    IJK_INITIALIZE_LOGGING();
    const auto kCount = 1000000;
    ijk::stopwatch sw;    

    int a = 0;


    std::mutex m;
    sw.start();
    for (auto i = 0; i < kCount; ++i) {
        std::unique_lock l(m);
        a = i; 
    }
    auto t = sw.elapsed<ijk::stopwatch::ns>();
    LOG_INFO("mutex op: {} ns", (double)t / (double)kCount);

    std::shared_mutex sm;
    sw.start();
    for (auto i = 0; i < kCount; ++i) {
        std::shared_lock l(sm);
        a = i; 
    }
    t = sw.elapsed<ijk::stopwatch::ns>();
    LOG_INFO("shared mutex op: {} ns", (double)t / (double)kCount);

    ijk::io_t io;
    sw.start();
    for (auto i = 0; i < kCount; ++i) {
        io.running_in_this_thread();
    }
    t = sw.elapsed<ijk::stopwatch::ns>();
    LOG_INFO("io.running_in_this_thread: {} ns", (double)t / (double)kCount);

    size_t item = 0;
    ijk::mpmc_blocking_q<size_t> blocking_q;
    sw.start();
    for (auto i = 0; i < kCount; ++i) {
        blocking_q.push(item);
        blocking_q.pop_nowait(item);
    }
    t = sw.elapsed<ijk::stopwatch::ns>();
    LOG_INFO("mpmc blocking q push/pop: {} ns", (double)t / (double)kCount);

    ijk::mpmc_bounded_queue<size_t> lockfree_q(128);
    sw.start();
    for (auto i = 0; i < kCount; ++i) {
        lockfree_q.enqueue((size_t &&)item);
        lockfree_q.dequeue(item);
    }
    t = sw.elapsed<ijk::stopwatch::ns>();
    LOG_INFO("mpmc lockfree q push/pop: {} ns", (double)t / (double)kCount);

    sw.start();
    for (auto i = 0; i < kCount; ++i) {
        auto arr = new char[i%513];
        delete[] arr;
    }
    t = sw.elapsed<ijk::stopwatch::ns>();
    LOG_INFO("new/delete: {} ns", (double)t / (double)kCount);

    std::any any;
    sw.start();
    for (auto i = 0; i < kCount; ++i) {
        any = i;
    }
    t = sw.elapsed<ijk::stopwatch::ns>();
    LOG_INFO("any: {} ns", (double)t / (double)kCount);

    std::unordered_map<std::string, int> umap;
    for (auto i = 0; i < 100; ++i) {
        umap.emplace(ijk::randomString(i%12 + 15), i);
    }
    sw.start();
    std::string ip = "172.0.0.1:4000";
    for (auto i = 0; i < kCount; ++i) {
        umap.find(ip);
    }
    t = sw.elapsed<ijk::stopwatch::ns>();
    LOG_INFO("unordered map<string,int>: {} ns", (double)t / (double)kCount);

    std::unordered_map<int, int> iumap;
    for (int i = 0; i < 100; ++i) {
        iumap.emplace(i, i);
    }
    sw.start();
    for (auto i = 0; i < kCount; ++i) {
        iumap.find(i);
    }
    t = sw.elapsed<ijk::stopwatch::ns>();
    LOG_INFO("unordered map<int,int>: {} ns", (double)t / (double)kCount);

    auto shared = std::make_shared<int>(1);
    auto weak = std::weak_ptr<int>(shared);
    sw.start();
    for (auto i = 0; i < kCount; ++i) {
        auto s = weak.lock();
        if (s != nullptr) {
        
        }
    }
    t = sw.elapsed<ijk::stopwatch::ns>();
    LOG_INFO("shared_ptr.lock: {} ns", (double)t / (double)kCount);

    sw.start();
    for (auto i = 0; i < kCount; ++i) {
        std::make_exception_ptr(std::logic_error("error"));
    }
    t = sw.elapsed<ijk::stopwatch::ns>();
    LOG_INFO("make_exception_ptr: {} ns", (double)t / (double)kCount);

    auto b = std::make_shared<base>();

    sw.start();
    for (auto i = 0; i < kCount; ++i) {
        auto p = b->shared_from_this();
        (void)p;
    }
    t = sw.elapsed<ijk::stopwatch::ns>();
    LOG_INFO("shared_from_this: {} ns", (double)t / (double)kCount);

    sw.start();
    for (auto i = 0; i < kCount; ++i) {
        auto d = std::static_pointer_cast<derived>(b->shared_from_this());
        (void)d;
    }
    t = sw.elapsed<ijk::stopwatch::ns>();
    LOG_INFO("shared_from_this + static_pointer_cast: {} ns", (double)t / (double)kCount);

    return 0;
}

