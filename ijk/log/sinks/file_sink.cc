/*
 * File Name: file_sink.cc
 * Author: Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Sun 14 Aug 2016 11:47:21 AM CST
 */

#include "file_sink.h"

#include <algorithm>
#include <ctime>
#include <iostream>

#include "ijk/base/details/fmt/time.h"

namespace ijk {

FileSink::FileSink(const std::string &basename, const std::string &extension,
                   size_t max_file_size, LogFormatter formatter)
    : basename_(basename),
      extension_(extension),
      max_file_size_(max_file_size),
      curr_file_size_(0),
      log_file_(),
      formatter_(std::move(formatter)) {
    std::replace(basename_.begin(), basename_.end(), '\\', '/');
    auto filename = mkFileName(basename_, extension_);
    changeLogFile(filename);
}

FileSink::~FileSink() {
    if (log_file_) log_file_->flush();
}

void FileSink::log(const LogMessage &msg) {
    if (!log_file_) return;
    auto content = formatter_(msg);

    curr_file_size_ += content.size();
    if (curr_file_size_ > max_file_size_) {
        curr_file_size_ = content.size();
        std::string name = log_file_->name();
        if (!changeLogFile(name)) {
            return;
        }
    }
    log_file_->append(content);
}

void FileSink::flush() {
    if (log_file_) log_file_->flush();
}

std::string FileSink::mkFileName(const std::string &basename,
                                 const std::string &extension) {
    auto now = std::chrono::system_clock::now();
    std::time_t time = log_clock_t::to_time_t(now);
    std::tm tm_time = fmt::localtime(time);
    auto filename =
        fmt::format("{}-{:04}-{:02}-{:02}-{:02}{:02}{:02}.{}", basename,
                    tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                    tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec, extension);

    return filename;
}

bool FileSink::changeLogFile(const std::string &filename) {
    if (log_file_) log_file_.reset();
    auto status = FileUtils::NewWritableFile(filename, log_file_, true);
    if (!status.ok())
        std::cerr << "failed to create file: " << filename << std::endl;
    return status.ok();
}

}  // namespace ijk
