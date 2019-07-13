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

#include "asio.h"

#include "ijk/base/noncopyable.h"

namespace ijk {
class io_t final : private Noncopyable {
public:
    io_t() : context_(), strand_(context_) {}
    ~io_t() = default;

    inline asio::io_context &context() { return context_; }
    inline asio::io_context::strand &strand() { return strand_; }

    inline bool running_in_this_thread() {
        auto id = std::this_thread::get_id();
        return id == owner_;
    }

    inline void run() {
        std::call_once(once_,
                       [this]() { owner_ = std::this_thread::get_id(); });
        context_.run();
    }

    inline void run_one() {
        std::call_once(once_,
                       [this]() { owner_ = std::this_thread::get_id(); });
        context_.run_one();
    }

private:
    asio::io_context context_;
    asio::io_context::strand strand_;
    std::thread::id owner_;
    std::once_flag once_;
};

/**
 * io_context pool
 */
class io_context_pool final : private Noncopyable {
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
            works_.emplace_back(io.context());

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
    std::vector<asio::io_context::work> works_;

    /// threads to run all of the io_context
    std::vector<std::thread> threads_;

    /// The next io_context to use for a connection.
    std::size_t next_ = 0;
};
}  // namespace ijk

#endif  // !__ASIO2_IOPOOL_HPP__
