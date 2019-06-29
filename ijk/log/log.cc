/*
 * File  : log.cc
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2018/12/23 11:19:53
 */

#include "log.h"
#include "logger.h"

#include <memory>

namespace ijk {
namespace details {

Logger *logger_instance{nullptr};

bool isLoggingInitialized() {
    return logger_instance != nullptr;
}

void saveLogMessage(LogMessage &&msg) {
    if (!isLoggingInitialized()) return;
    logger_instance->save(std::move(msg));
}

void shutdownLogging() {
    logger_instance = nullptr;
}

void shutdownLoggingForActiveOnly(Logger *active) {
    if (logger_instance == active) logger_instance = nullptr;
}

}  // namespace details

void initializeLogging(Logger *logger) {
    details::logger_instance = logger;
}

}  // namespace ijk
