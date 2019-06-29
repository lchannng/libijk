/*
 * File  : log.h
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2018/12/23 11:19:55
 */

#pragma once

#include "log_common.h"
#include "log_writer.h"

namespace ijk {

class Logger;

namespace details {
bool isLoggingInitialized();
void saveLogMessage(LogMessage &&msg);

// Shuts down logging. No object cleanup but further LOG_XXX(...) calls will be
// ignored.
void shutdownLogging();

// Shutdown logging, but ONLY if the active logger corresponds to the one
// currently initialized
void shutdownLoggingForActiveOnly(Logger *active);
}  // namespace details

void initializeLogging(Logger *logger);

#define IJK_LOG_MESSAGE(level, ...)           \
    if (ijk::details::isLoggingInitialized()) \
    ijk::LogWriter(level, __FILE__, __LINE__).format(__VA_ARGS__)

#define IJK_LOG_MESSAGE_IF(boolean_expression, level, ...)            \
    if (ijk::details::isLoggingInitialized() && (boolean_expression)) \
    ijk::LogWriter(level, __FILE__, __LINE__).format(__VA_ARGS__)

#define LOG_DEBUG(...) IJK_LOG_MESSAGE(ijk::LogLevel::kDebug, __VA_ARGS__)

#define LOG_INFO(...) IJK_LOG_MESSAGE(ijk::LogLevel::kInfo, __VA_ARGS__)

#define LOG_WARN(...) IJK_LOG_MESSAGE(ijk::LogLevel::kWarning, __VA_ARGS__)

#define LOG_ERROR(...) IJK_LOG_MESSAGE(ijk::LogLevel::kError, __VA_ARGS__)

#define LOG_FATAL(...) IJK_LOG_MESSAGE(ijk::LogLevel::kFatal, __VA_ARGS__)

#define LOG_DEBUG_IF(boolean_expression, ...) \
    IJK_LOG_MESSAGE_IF((boolean_expression), ijk::LogLevel::kDebug, __VA_ARGS__)

#define LOG_INFO_IF(boolean_expression, ...) \
    IJK_LOG_MESSAGE_IF((boolean_expression), ijk::LogLevel::kInfo, __VA_ARGS__)

#define LOG_WARN_IF(boolean_expression, ...)                          \
    IJK_LOG_MESSAGE_IF((boolean_expression), ijk::LogLevel::kWarning, \
                       __VA_ARGS__)

#define LOG_ERROR_IF(boolean_expression, ...) \
    IJK_LOG_MESSAGE_IF((boolean_expression), ijk::LogLevel::kError, __VA_ARGS__)

#define LOG_FATAL_IF(boolean_expression, ...) \
    IJK_LOG_MESSAGE_IF((boolean_expression), ijk::LogLevel::kFatal, __VA_ARGS__)

}  // namespace ijk
