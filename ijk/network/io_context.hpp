/*
 * File  : io_context_pool.h
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/07/09 09:55:26
 */

#pragma once
#ifndef __ASIO2_IOPOOL_HPP__
#define __ASIO2_IOPOOL_HPP__

#include <chrono>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>

#include "ijk/base/noncopyable.h"
#include "ijk/network/asio_compatibility.hpp"

namespace ijk {
class io_t final : private noncopyable {
public:
    io_t() = default;
    ~io_t() = default;

    inline asio::io_context &context() { return context_; }

    inline bool running_in_this_thread() {
        auto id = std::this_thread::get_id();
        return id == owner_;
    }

    inline void run() {
        try_init();
        context_.run();
    }

    template <typename Rep, typename Period>
    std::size_t run_for(
        const std::chrono::duration<Rep, Period>& rel_time)
    {
        try_init();
        return context_.run_for(rel_time);
    }

    inline std::size_t poll() {
        try_init();
        return context_.poll();
    }

    inline std::size_t poll_one() {
        try_init();
        return context_.poll_one();
    }

    template <typename T>
    inline void dispatch(T &&func) {
        asio::dispatch(context_, std::forward<T>(func));
    }

    template <typename T>
    inline void post(T &&func) {
        asio::post(context_, std::forward<T>(func));
    }

    template <typename T>
    inline void push(T &&func) {
        asio::post(context_, std::forward<T>(func));
    }

private:
    inline void try_init() {
        std::call_once(once_,
                       [this]() { owner_ = std::this_thread::get_id(); });
    }

private:
    asio::io_context context_;
    std::thread::id owner_;
    std::once_flag once_;
};

/**
 * io_context pool
 */
class io_context_pool final : private noncopyable {
public:
    /**
     * @constructor
     * @param    : concurrency - the pool size,default is double the number of
     * CPU cores
     */
    explicit io_context_pool(std::size_t concurrency = 0)
        : io_contexts_(concurrency == 0 ? std::thread::hardware_concurrency()
                                        : concurrency) {}

    /**
     * @destructor
     */
    ~io_context_pool() = default;

    /**
     * @function : run all io_context objects in the pool.
     */
    void run(bool async = false) {
        if (!works_.empty() || !threads_.empty()) return;

        threads_.reserve(io_contexts_.size());
        works_.reserve(io_contexts_.size());

        // Create a pool of threads to run all of the io_contexts.
        for (auto &io : io_contexts_) {
            works_.emplace_back(io.context().get_executor());

            // start work thread
            threads_.emplace_back([&io]() { io.run(); });
        }

        if (!async) join();
    }

    void stop() { works_.clear(); }

    void join() {
        for (auto &t : threads_) {
            t.join();
        }
        threads_.clear();
    }

    inline io_t &get(std::size_t index = static_cast<std::size_t>(-1)) {
        // Use a round-robin scheme to choose the next io_context to use.
        return io_contexts_[index < io_contexts_.size()
                                ? index
                                : ((++(next_)) % io_contexts_.size())];
    }

private:
    /// The pool of io_context.
    std::vector<io_t> io_contexts_;

    // Give all the io_contexts work to do so that their run() functions will
    // not exit until they are explicitly stopped.
    std::vector<asio::executor_work_guard<asio::io_context::executor_type>>
        works_;

    /// threads to run all of the io_context
    std::vector<std::thread> threads_;

    /// The next io_context to use for a connection.
    std::size_t next_ = 0;
};
}  // namespace ijk

#endif  // !__ASIO2_IOPOOL_HPP__
