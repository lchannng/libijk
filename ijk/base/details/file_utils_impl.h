/*
 * File Name: file_utils_impl.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2017/1/27 18:35:58
 */

#ifndef _FILE_UTILS_IMPL_H
#define _FILE_UTILS_IMPL_H

#include <sys/stat.h>
#include <algorithm>
#include <cstdio>
#include "../file_utils.h"

namespace ijk {

namespace details {

enum IOError {
    ReadError = 1,
    WriteError,
    SeekError,
    FlushError,
    FileNotExisted,
    FailedToCreate,
};

class IOErrorCategory : public std::error_category {
public:
    virtual const char *name() const noexcept override {
        return "IOErrorCategory";
    }

    virtual std::string message(int val) const override {
        return std::string("io error:") + std::to_string(val);
    }
};

std::error_code makeErrorCode(IOError error) {
    static IOErrorCategory category;
    return std::error_code((int)error, category);
}

class SequentialFileImpl : public SequentialFile {
private:
    FILE *fp_;
    std::string fname_;

public:
    SequentialFileImpl(FILE *fp, const std::string &fname)
        : fp_(fp), fname_(fname) {}
    ~SequentialFileImpl() {
        if (fp_) {
            fclose(fp_);
        }
    }
    void Read(size_t n, char *buf, std::string_view &result,
              std::error_code &ec) override {
        size_t nread = fread(buf, 1, n, fp_);
        if (nread != n) {
            if (feof(fp_)) {
                // Reach the end of file
            } else {
                // A partial read with an error
                ec = makeErrorCode(ReadError);
                return;
                ;
            }
        }
        result = std::string_view(buf, nread);
    }

    void Skip(size_t n, std::error_code &ec) override {
        if (fseek(fp_, static_cast<long>(n), SEEK_CUR)) {
            ec = makeErrorCode(ReadError);
        }
    }
};

class WritableFileImpl : public WritableFile {
private:
    FILE *fp_;
    std::string fname_;
    bool dirty_{false};

public:
    WritableFileImpl(FILE *fp, const std::string &fname)
        : fp_(fp), fname_(fname) {}

    ~WritableFileImpl() {
        if (fp_) {
            std::error_code ec;
            flush(ec);
            fclose(fp_);
        }
    }

    void append(const std::string_view &data, std::error_code &ec) override {
        if (data.empty()) return;

        size_t res = fwrite(data.data(), 1, data.size(), fp_);
        if (res != data.size()) {
            ec = makeErrorCode(WriteError);
            return;
        }
        dirty_ = true;
    }

    void flush(std::error_code &ec) override {
        if (dirty_) {
            if (fflush(fp_)) {
                ec = makeErrorCode(FlushError);
                return;
            }
            dirty_ = false;
        }
    }

    const std::string &name() override { return fname_; }
};

inline void ReadFile(SequentialFile::Ptr &file, std::string &out,
                     std::error_code &ec) {
    out.clear();
    char buf[8192];
    std::string_view fragment;
    while (true) {
        file->Read(sizeof(buf), buf, fragment, ec);
        if (ec) {
            return;
        }
        if (fragment.empty()) {
            break;
        }
        out.append(fragment.data(), fragment.size());
    }
}

}  // namespace details

inline SequentialFile::Ptr FileUtils::NewSequentialFile(
    const std::string &fname, bool binary /* = false*/) {
    using namespace std;

    std::string mode = "r";
    if (binary) mode += "b";

    FILE *fp = fopen(fname.c_str(), mode.c_str());
    if (!fp) {
        return nullptr;
    }
    return std::make_unique<details::SequentialFileImpl>(fp, fname);
}

inline WritableFile::Ptr FileUtils::NewWritableFile(const std::string &fname,
                                                    bool trunc /* = false*/,
                                                    bool binary /* = false*/) {
    using namespace std;

    std::string mode = "a";
    if (trunc) mode = "w";
    if (binary) mode += "b";

    FILE *fp = fopen(fname.c_str(), mode.c_str());
    if (!fp) {
        return nullptr;
    }
    return std::make_unique<details::WritableFileImpl>(fp, fname);
}

inline std::string FileUtils::ReadAllText(const std::string &fname,
                                          std::error_code &ec) {
    auto seq_file = NewSequentialFile(fname, false);
    if (!seq_file) {
        ec = makeErrorCode(details::FileNotExisted);
        return std::string();
    }
    std::string res;
    details::ReadFile(seq_file, res, ec);
    return res;
}

inline std::string FileUtils::ReadAllBytes(const std::string &fname,
                                           std::error_code &ec) {
    auto seq_file = NewSequentialFile(fname, true);
    if (!seq_file) {
        ec = makeErrorCode(details::FileNotExisted);
        return std::string();
    }
    std::string res;
    details::ReadFile(seq_file, res, ec);
    return res;
}

inline void FileUtils::WriteAllText(const std::string &fname,
                                    const std::string_view &what,
                                    std::error_code &ec) {
    auto file = NewWritableFile(fname, true, false);
    if (!file) {
        ec = makeErrorCode(details::FailedToCreate);
        return;
    }
    file->append(what, ec);
}

inline void FileUtils::WriteAllBytes(const std::string &fname,
                                     const std::string_view &what,
                                     std::error_code &ec) {
    auto file = NewWritableFile(fname, true, true);
    if (!file) {
        ec = makeErrorCode(details::FailedToCreate);
        return;
    }
    file->append(what, ec);
}

inline std::string FileUtils::GetBaseName(const std::string &fname) {
    return std::string(std::find_if(fname.rbegin(), fname.rend(),
                                    [](std::string::value_type ch) {
                                        return '\\' == ch || '/' == ch;
                                    })
                           .base(),
                       fname.end());
}

inline bool FileUtils::Exists(const std::string &fname) {
    return Exists(fname.c_str());
}

inline bool FileUtils::Exists(const char *fname) {
#if defined(WIN32) || defined(WIN64)
#define S_ISREG(m) (((m)&S_IFMT) == S_IFREG)
#define S_ISDIR(m) (((m)&S_IFMT) == S_IFDIR)
#endif

    struct stat info;
    if (::stat(fname, &info) != 0) return false;
    return S_ISREG(info.st_mode);
}

inline void FileUtils::Remove(const std::string_view &fname,
                              std::error_code &ec) {
    int res = std::remove(fname.data());
    if (res == 0) return;
    ec = makeErrorCode(details::FileNotExisted);
}

}  // namespace ijk

#endif
