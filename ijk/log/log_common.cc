/*
 * File  : log_common.cc
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2018/12/24 23:50:06
 */

#include "ijk/base/fmt.h"

#include "fmt_helper.h"
#include "log_common.h"

namespace ijk {

inline const char *EOL() {
#if defined(_WIN32) || defined(_WIN64)
    return "\r\n";
#elif defined(__APPLE__) || defined(__MACH__)
    return "\n";
#else
    return "\n";
#endif
}

static void localTime(const std::time_t &time_tt, std::tm &tm) {
#ifdef _WIN32
    localtime_s(&tm, &time_tt);
#else
    localtime_r(&time_tt, &tm);
#endif
}

inline static const char *extractFileName(const char *fullname) {
#if defined(_WIN32) || defined(_WIN64)
    auto name = strrchr(fullname, '\\');
#else
    auto name = strrchr(fullname, '/');
#endif
    return name ? name + 1 : fullname;
}

LogMessage::LogMessage(LogLevel ls, const char *f, int l) noexcept
    : file(f), line(l), level(ls), text(), time(log_clock_t::now()) {}

LogMessage::LogMessage(const LogMessage &other) noexcept
    : file(other.file),
      line(other.line),
      level(other.level),
      text(other.text),
      time(other.time) {}

LogMessage::LogMessage(LogMessage &&other) noexcept
    : file(other.file),
      line(other.line),
      level(other.level),
      text(std::move(other.text)),
      time(std::move(other.time)) {}

std::string DefaultFormatter::operator()(const LogMessage &msg) {
    using std::chrono::duration_cast;
    using std::chrono::milliseconds;
    using std::chrono::seconds;

    // cache the date/time part for the next second.
    auto duration = msg.time.time_since_epoch();
    auto secs = duration_cast<seconds>(duration);

    if (secs != cached_timestamp_) {
        std::time_t time = log_clock_t::to_time_t(msg.time);
        std::tm tm_time;
        localTime(time, tm_time);

        cached_datetime_ = fmt::format(
            "{:04}-{:02}-{:02} {:02}:{:02}:{:02}", tm_time.tm_year + 1900,
            tm_time.tm_mon + 1, tm_time.tm_mday, tm_time.tm_hour,
            tm_time.tm_min, tm_time.tm_sec);
        cached_timestamp_ = secs;
    }

    fmt::memory_buffer dest;

    fmt_helper::append_string_view(cached_datetime_, dest);
    dest.push_back('.');

    auto millis = fmt_helper::time_fraction<milliseconds>(msg.time);
    fmt_helper::pad3(static_cast<uint32_t>(millis.count()), dest);

    dest.push_back(' ');
    dest.push_back('|');
    dest.push_back(' ');

    constexpr static const char *level_names[] = {
        "", "DEBUG", "INFO ", "WARN ", "ERROR", "FATAL",
    };

    // fmt_helper::append_string_view(level::to_c_str(msg.level), dest);
    fmt_helper::append_string_view(level_names[msg.level], dest);
    dest.push_back(' ');
    dest.push_back('|');
    dest.push_back(' ');

    // fmt_helper::append_string_view(msg.msg(), dest);
    fmt_helper::append_string_view(msg.text, dest);
    dest.push_back(' ');

    // file and line
    dest.push_back('(');
    fmt_helper::append_string_view(extractFileName(msg.file), dest);
    dest.push_back(':');
    fmt_helper::append_int(msg.line, dest);
    dest.push_back(')');

    fmt_helper::append_string_view(EOL(), dest);

    return std::string(dest.data(), dest.size());
}

void LogSink::flush() {}

}  // namespace ijk
