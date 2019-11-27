/*
 * File  : log.h
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2018/12/23 11:19:55
 */

#pragma once

#define SPDLOG_ACTIVE_LEVEL 1

#include "spdlog/spdlog.h"

#include "spdlog/async.h"
#include "spdlog/fmt/bundled/chrono.h"
#include "spdlog/fmt/bundled/ostream.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace ijk {

class Logging final {
public:
    ~Logging() = default;

    static Logging &instance() {
        static Logging log;
        return log;
    }

    spdlog::logger *logger() { return _default_logger.get(); }

    void initialize() {
        spdlog::init_thread_pool(10240, 1);
        _default_logger =
            spdlog::create_async<spdlog::sinks::stdout_color_sink_mt>("main");
    }

private:
    Logging() = default;
    Logging(const Logging &) = delete;
    Logging &operator=(const Logging &) = delete;

private:
    std::shared_ptr<spdlog::logger> _default_logger;
};

#define IJK_INITIALIZE_LOGGING() ijk::Logging::instance().initialize()

#define IJK_LOG_MESSAGE(level, ...) \
    SPDLOG_LOGGER_CALL(ijk::Logging::instance().logger(), level, __VA_ARGS__)

#define IJK_LOG_MESSAGE_IF(boolean_expression, level, ...) \
    if (boolean_expression)                                \
    SPDLOG_LOGGER_CALL(ijk::Logging::instance().logger(), level, __VA_ARGS__)

#define LOG_DEBUG(...) IJK_LOG_MESSAGE(spdlog::level::debug, __VA_ARGS__)

#define LOG_INFO(...) IJK_LOG_MESSAGE(spdlog::level::info, __VA_ARGS__)

#define LOG_WARN(...) IJK_LOG_MESSAGE(spdlog::level::warn, __VA_ARGS__)

#define LOG_ERROR(...) IJK_LOG_MESSAGE(spdlog::level::err, __VA_ARGS__)

#define LOG_FATAL(...) IJK_LOG_MESSAGE(spdlog::level::critical, __VA_ARGS__)

#define LOG_DEBUG_IF(boolean_expression, ...) \
    IJK_LOG_MESSAGE_IF((boolean_expression), spdlog::level::debug, __VA_ARGS__)

#define LOG_INFO_IF(boolean_expression, ...) \
    IJK_LOG_MESSAGE_IF((boolean_expression), spdlog::level::info, __VA_ARGS__)

#define LOG_WARN_IF(boolean_expression, ...) \
    IJK_LOG_MESSAGE_IF((boolean_expression), spdlog::level::warn, __VA_ARGS__)

#define LOG_ERROR_IF(boolean_expression, ...) \
    IJK_LOG_MESSAGE_IF((boolean_expression), spdlog::level::err, __VA_ARGS__)

#define LOG_FATAL_IF(boolean_expression, ...)                         \
    IJK_LOG_MESSAGE_IF((boolean_expression), spdlog::level::critical, \
                       __VA_ARGS__)

}  // namespace ijk
