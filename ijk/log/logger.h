/*
 * File  : logger.h
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2018/12/22 23:07:29
 */

#pragma once

#ifndef _IJK_LOGGER_H
#define _IJK_LOGGER_H

#include "log.h"
#include "log_common.h"

#include "ijk/base/noncopyable.h"

namespace ijk {

class Logger final : private Noncopyable {
    class Impl;

public:
    Logger();
    ~Logger();
    void save(LogMessage &&msg);
    void addLogSink(LogSinkPtr sink);

    static std::unique_ptr<Logger> createDefaultLogger();

private:
    Impl *impl_{nullptr};
};  // class Logger

#define IJK_INITIALIZE_LOGGING()                           \
    auto _ijk_logger = ijk::Logger::createDefaultLogger(); \
    ijk::initializeLogging(_ijk_logger.get())

}  // namespace ijk

#endif /* ifndef _IJK_LOGGER_H */
