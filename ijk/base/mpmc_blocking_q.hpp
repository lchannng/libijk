/** ==========================================================================
 * 2010 by KjellKod.cc. This is PUBLIC DOMAIN to use at your own risk and comes
 * with no warranties. This code is yours to share, use and modify with no
 * strings attached and no restrictions or obligations.
 *
 * For more information see g3log/LICENSE or refer refer to http://unlicense.org
 * ============================================================================
 *
 * Example of a normal std::queue protected by a mutex for operations,
 * making it safe for thread communication, using std::mutex from C++0x with
 * the help from the std::thread library from JustSoftwareSolutions
 * ref: http://www.stdthread.co.uk/doc/headers/mutex.html
 *
 * This exampel  was totally inspired by Anthony Williams lock-based data
 * structures in Ref: "C++ Concurrency In Action"
 * http://www.manning.com/williams */

#pragma once

#include <atomic>
#include <condition_variable>
#include <exception>
#include <mutex>
#include <queue>

namespace ijk {

/** Multiple producer, multiple consumer thread safe queue
 * Since 'return by reference' is used this queue won't throw */
template <typename T>
class mpmc_blocking_q {
public:
    using container_type = std::deque<T>;

    mpmc_blocking_q() : shutdown_(false) {}

    void push(T item) {
        {
            std::lock_guard<std::mutex> lock(m_);
            queue_.push_back(std::move(item));
        }
        data_cond_.notify_one();
    }

    /// \return immediately, with true if successful retrieval
    bool pop_nowait(T &popped_item) {
        std::lock_guard<std::mutex> lock(m_);
        if (queue_.empty() || shutdown_) {
            return false;
        }
        popped_item = std::move(queue_.front());
        queue_.pop_front();
        return true;
    }

    bool try_pop(T &popped_item, std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(m_);
        if (!data_cond_.wait_for(lock, timeout, [this] {
                return !queue_.empty() || shutdown_;
            })) {
            return false;
        }

        if (queue_.empty() || shutdown_)
            return false;

        popped_item = std::move(queue_.front());
        queue_.pop_front();
        return true;
    }

    /// Try to retrieve, if no items, wait till an item is available and try
    /// again
    bool wait_and_pop(T &popped_item) {
        std::unique_lock<std::mutex> lock(m_);
        while (queue_.empty() && !shutdown_) {
            data_cond_.wait(lock);
            //  This 'while' loop is equal to
            //  data_cond_.wait(lock, [](bool result){return !queue_.empty();});
        }

        if (queue_.empty() || shutdown_)
            return false;

        popped_item = std::move(queue_.front());
        queue_.pop_front();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(m_);
        return queue_.empty();
    }

    unsigned size() const {
        std::lock_guard<std::mutex> lock(m_);
        return queue_.size();
    }

    bool swap(container_type &other) {
        std::lock_guard<std::mutex> lock(m_);
        if (queue_.empty())
            return false;
        other.swap(queue_);
        return true;
    }

    void shutdown() {
        std::lock_guard<std::mutex> lock(m_);
        while (!queue_.empty()) {
            queue_.pop_front();
        }

        shutdown_ = true;
        data_cond_.notify_all();
    }

private:
    container_type queue_;
    mutable std::mutex m_;
    std::condition_variable data_cond_;
    std::atomic<bool> shutdown_;

    mpmc_blocking_q &operator=(const mpmc_blocking_q &) = delete;
    mpmc_blocking_q(const mpmc_blocking_q &other) = delete;

};

}  // namespace ijk
