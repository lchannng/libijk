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
#include "status.h"

namespace ijk {

class Slice;

class SequentialFile {
public:
    using Ptr = std::unique_ptr<SequentialFile>;
    SequentialFile() {}
    virtual ~SequentialFile() {}
    virtual Status Read(size_t n, char *buf, Slice *result) = 0;
    virtual Status Skip(size_t n) = 0;
    SequentialFile(const SequentialFile &) = delete;
    void operator=(const SequentialFile &) = delete;
};

class WritableFile {
public:
    using Ptr = std::unique_ptr<WritableFile>;
    WritableFile() {}
    virtual ~WritableFile() {}
    virtual Status append(const Slice &data) = 0;
    virtual Status flush() = 0;
    virtual const std::string &name() = 0;
    WritableFile(const WritableFile &) = delete;
    void operator=(const WritableFile &) = delete;
};

class FileUtils {
public:
    static Status NewSequentialFile(const std::string &fname,
                                    SequentialFile::Ptr &res,
                                    bool binary = false);
    static Status NewWritableFile(const std::string &fname,
                                  WritableFile::Ptr &res, bool trunc = false,
                                  bool binary = false);
    static Status ReadAllText(const std::string &fname, std::string *out);
    static Status WriteAllText(const std::string &fname, const Slice &what);
    static Status ReadAllBytes(const std::string &fname, std::string *out);
    static Status WriteAllBytes(const std::string &fname, const Slice &what);
    static std::string GetBaseName(const std::string &fname);
    static bool Exists(const std::string &fname);
    static bool Exists(const char *fname);
    static Status Remove(const std::string &fname);
    static Status Remove(const char *fname);
};  // class FileUtils

}  // namespace ijk

#include "./details/file_utils_impl.h"

#endif
