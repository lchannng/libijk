/*
 * File  : console_sink.h
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2018/12/24 19:31:34
 */

#pragma once

#include "../log_common.h"

namespace ijk {

class ConsoleSink final : public LogSink {
public:
    ConsoleSink(LogFormatter formatter);
    ~ConsoleSink() = default;
    virtual void log(const LogMessage &msg) override;

private:
    LogFormatter formatter_;
};  // class ConsoleSink

}  // namespace ijk
