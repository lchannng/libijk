/*
 * File Name: thread_pool.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2018/6/5 16:34:10
 */

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace ijk {

class threadpool final {
public:
    threadpool(size_t nthreads);
    ~threadpool();
    threadpool(const threadpool &) = delete;
    threadpool &operator=(const threadpool &) = delete;

    template <class F, class... Args>
    auto submit(F &&f, Args &&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>;

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void(void)>> tasks_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stop_{false};
};

inline threadpool::threadpool(size_t nthreads) {
    nthreads = nthreads > 0 ? nthreads : 1;

    for (int i = 0; i < nthreads; ++i) {
        workers_.emplace_back([this] {
            for (;;) {
                std::function<void(void)> task;
                {
                    std::unique_lock<std::mutex> lock(mutex_);
                    cv_.wait(lock, [this] {
                        return this->stop_ || !this->tasks_.empty();
                    });
                    if (this->stop_ && this->tasks_.empty()) return;
                    task = std::move(this->tasks_.front());
                    this->tasks_.pop();
                }
                task();
            }
        });
    }
}

inline threadpool::~threadpool() {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        stop_ = true;
    }
    cv_.notify_all();
    for (auto &worker : workers_) {
        if (worker.joinable()) worker.join();
    }
}

template <typename F, typename... Args>
inline auto threadpool::submit(F &&f, Args &&... args)
    -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (stop_)
            throw std::runtime_error("Submit task to stopped thread pool");
        tasks_.emplace([task] { (*task)(); });
    }
    cv_.notify_one();
    return res;
}
}  // namespace ijk

#endif
