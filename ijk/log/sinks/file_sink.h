/*
 * File Name: file_sink.h
 * Author: Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Sun 14 Aug 2016 10:41:13 AM CST
 */

#ifndef _FILE_SINK_H
#define _FILE_SINK_H

#include "../log_common.h"

#include "ijk/base/file_utils.h"

namespace ijk {

class FileSink : public LogSink {
public:
    FileSink(const std::string &basename, const std::string &extension,
             size_t max_file_size, LogFormatter);
    ~FileSink();
    void log(const LogMessage &msg) override;
    void flush() override;

private:
    static std::string mkFileName(const std::string &basename,
                                  const std::string &extension);
    bool changeLogFile(const std::string &name);

private:
    std::string basename_;
    std::string extension_;
    size_t max_file_size_;
    size_t curr_file_size_;
    WritableFile::Ptr log_file_;
    LogFormatter formatter_;
};

}  // namespace ijk

#endif
