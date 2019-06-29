/*
 * File  : log_writer.cc
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2018/12/23 22:27:09
 */

#include "log_writer.h"
#include "log.h"
#include "logger.h"

namespace ijk {

LogWriter::LogWriter(LogLevel ls, const char *f, int l) : msg_(ls, f, l) {}

LogWriter::~LogWriter() {
    details::saveLogMessage(std::move(msg_));
}

}  // namespace ijk
