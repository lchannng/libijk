/*
 * File  : benchmark.cc
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/08/07 19:40:12
 */

#include "ijk/base/logging.hpp"
#include "ijk/base/mpmc_blocking_q.hpp"
#include "ijk/base/mpmc_queue.h"
#include "ijk/base/random.h"
#include "ijk/base/stopwatch.h"
#include "ijk/network/io_context_pool.hpp"

#include <any>
#include <iostream>
#include <mutex>
#include <shared_mutex>

int main(int argc, char *argv[])
{
    IJK_INITIALIZE_LOGGING();
    const size_t kCount = 1000000;
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

    return 0;
}

