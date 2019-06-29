/*
 * File  : logger.cc
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2018/12/22 23:07:34
 */

#include "logger.h"
#include "fmt_helper.h"
#include "log.h"
#include "sinks/console_sink.h"

#include "ijk/base/gsl.h"
#include "ijk/base/mpmc_blocking_q.hpp"

#include <atomic>
#include <functional>
#include <list>
#include <memory>

namespace ijk {

class Logger::Impl {
public:
    Impl() : worker_thread_(&Impl::workerLoop, this) {}

    ~Impl() {
        job_queue_.push([this]() { done_ = true; });
    }

    void addLogSink(LogSinkPtr sink) {
        job_queue_.push(
            [this, s(std::move(sink))]() { sinks_.push_back(std::move(s)); });
    }

    void bgSave(LogMessage &&msg) {
        job_queue_.push([this, m = std::move(msg)]() {
            for (auto &sink : sinks_) {
                sink->log(m);
            }
            dirty_ = true;
        });
    }

private:
    void workerLoop() {
        using namespace std::chrono;
        while (!done_) {
            Job job;
            if (job_queue_.try_pop(job, milliseconds(30))) {
                job();
                continue;
            }
            auto now = steady_clock::now();
            if (dirty_ && now - last_flush_time_ > milliseconds(200)) {
                dirty_ = false;
                last_flush_time_ = now;
                for (auto &sink : sinks_) sink->flush();
            }
        }
    }

private:
    using Job = std::function<void()>;

    std::atomic<LogLevel> log_serverity_{LogLevel::kAll};
    std::list<LogSinkPtr> sinks_;
    mpmc_blocking_q<Job> job_queue_;
    bool done_{false};
    bool dirty_{false};
    std::chrono::steady_clock::time_point last_flush_time_{};
    gsl::joining_thread worker_thread_;  // must be the last member
};

Logger::Logger() : impl_(new Impl()) {}

Logger::~Logger() {
    details::shutdownLoggingForActiveOnly(this);
    delete impl_;
}

void Logger::save(LogMessage &&msg) {
    impl_->bgSave(std::move(msg));
}

void Logger::addLogSink(LogSinkPtr sink) {
    impl_->addLogSink(std::move(sink));
}

std::unique_ptr<ijk::Logger> Logger::createDefaultLogger() {
    auto ptr = std::make_unique<Logger>();
    auto sink = std::make_shared<ConsoleSink>(DefaultFormatter());
    ptr->addLogSink(std::move(sink));
    return std::move(ptr);
}

}  // namespace ijk