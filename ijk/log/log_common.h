/*
 * File  : common.h
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2018/12/23 20:49:49
 */

#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <string>

namespace ijk {

enum LogLevel {
    kAll,
    kDebug,
    kInfo,
    kWarning,
    kError,
    kFatal,
};

using log_clock_t = std::chrono::system_clock;

struct LogMessage {
    const char *file;
    int line{0};
    LogLevel level;
    std::string text;
    log_clock_t::time_point time;

    LogMessage(LogLevel ls, const char *f, int l) noexcept;
    LogMessage(LogMessage &&other) noexcept;
    LogMessage(const LogMessage &other) noexcept;
};

using LogFormatter = std::function<std::string(const LogMessage &)>;

class DefaultFormatter final {
public:
    DefaultFormatter() = default;
    ~DefaultFormatter() = default;
    std::string operator()(const LogMessage &msg);

private:
    std::chrono::seconds cached_timestamp_{0};
    std::string cached_datetime_;
};

class LogSink {
public:
    LogSink() = default;
    virtual ~LogSink() {}
    virtual void log(const LogMessage &msg) = 0;
    virtual void flush();
};

using LogSinkPtr = std::shared_ptr<LogSink>;

}  // namespace ijk
