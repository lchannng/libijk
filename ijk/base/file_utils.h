/*
 * File Name: file_utils.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2017/1/27 18:13:28
 */

#ifndef _FILE_UTILS_H
#define _FILE_UTILS_H

#include <memory>
#include <string>
#include <string_view>
#include <system_error>

namespace ijk {

class Slice;

class SequentialFile {
public:
    using Ptr = std::unique_ptr<SequentialFile>;
    SequentialFile() {}
    virtual ~SequentialFile() {}
    virtual void Read(size_t n, char *buf, std::string_view &result, std::error_code &ec) = 0;
    virtual void Skip(size_t n, std::error_code &ec) = 0;
    SequentialFile(const SequentialFile &) = delete;
    void operator=(const SequentialFile &) = delete;
};

class WritableFile {
public:
    using Ptr = std::unique_ptr<WritableFile>;
    WritableFile() {}
    virtual ~WritableFile() {}
    virtual void append(const std::string_view &data, std::error_code &ec) = 0;
    virtual void flush(std::error_code &ec) = 0;
    virtual const std::string &name() = 0;
    WritableFile(const WritableFile &) = delete;
    void operator=(const WritableFile &) = delete;
};

class FileUtils {
public:
    static SequentialFile::Ptr NewSequentialFile(const std::string &fname,
                                                 bool binary = false);
    static WritableFile::Ptr NewWritableFile(const std::string &fname,
                                             bool trunc = false,
                                             bool binary = false);
    static std::string ReadAllText(const std::string &fname,
                                   std::error_code &ec);
    static void WriteAllText(const std::string &fname,
                             const std::string_view &what, std::error_code &ec);
    static std::string ReadAllBytes(const std::string &fname,
                                    std::error_code &ec);
    static void WriteAllBytes(const std::string &fname,
                              const std::string_view &what,
                              std::error_code &ec);
    static std::string GetBaseName(const std::string &fname);
    static bool Exists(const std::string &fname);
    static bool Exists(const char *fname);
    static void Remove(const std::string_view &fname, std::error_code &ec);
};  // class FileUtils

}  // namespace ijk

#include "./details/file_utils_impl.h"

#endif
