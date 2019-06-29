/*
 * File  : log_writer.h
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2018/12/23 11:38:33
 */

#pragma once

#include "ijk/base/fmt.h"

#include "log_common.h"

namespace ijk {

class Logger;

class LogWriter final {
public:
    LogWriter(LogLevel ls, const char *f, int l);
    ~LogWriter();

    template <typename... Args>
    void format(const char *format, Args &&... args) {
        auto content = fmt::format(format, std::forward<Args>(args)...);
        msg_.text.swap(content);
    }

    template <typename... Args>
    void format(const std::string &format, Args &&... args) {
        this->format(format.c_str(), std::forward<Args>(args)...);
    }

private:
    LogMessage msg_;
};

}  // namespace ijk
