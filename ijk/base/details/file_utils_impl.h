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
#include "../slice.h"

namespace ijk {

namespace details {

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

    Status Read(size_t n, char *buf, Slice *result) override {
        size_t nread = fread(buf, 1, n, fp_);
        if (nread != n) {
            if (feof(fp_)) {
                // Reach the end of file
            } else {
                // A partial read with an error
                return Status::IOError("A partial read with an error");
                ;
            }
        }
        *result = Slice(buf, nread);
        return Status::OK();
    }

    Status Skip(size_t n) override {
        if (fseek(fp_, static_cast<long>(n), SEEK_CUR)) {
            return Status::IOError("fseek error");
        }
        return Status::OK();
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
            flush();
            fclose(fp_);
        }
    }

    Status append(const Slice &data) override {
        if (data.empty()) return Status::OK();

        size_t res = fwrite(data.data(), 1, data.size(), fp_);
        if (res != data.size()) {
            return Status::IOError("fwrite error.");
        }
        dirty_ = true;
        return Status::OK();
    }

    Status flush() override {
        if (dirty_) {
            if (fflush(fp_)) {
                return Status::IOError("flush error.");
            }
            dirty_ = false;
        }
        return Status::OK();
    }

    const std::string &name() override { return fname_; }
};

inline Status ReadFile(SequentialFile::Ptr &file, std::string *out) {
    out->clear();
    char buf[8192];
    Slice fragment;
    while (true) {
        auto res = file->Read(sizeof(buf), buf, &fragment);
        if (!res.ok()) {
            return res;
        }
        if (fragment.empty()) {
            break;
        }
        out->append(fragment.data(), fragment.size());
    }
    return Status::OK();
}

}  // namespace details

inline Status FileUtils::NewSequentialFile(const std::string &fname,
                                           SequentialFile::Ptr &res,
                                           bool binary /* = false*/) {
    using namespace std;

    std::string mode = "r";
    if (binary) mode += "b";

    FILE *fp = fopen(fname.c_str(), mode.c_str());
    if (!fp) {
        // error msg
        return Status::IOError("Failed to open file");
    }
    SequentialFile *seq_file = new details::SequentialFileImpl(fp, fname);
    res.reset(seq_file);
    return Status::OK();
}

inline Status FileUtils::NewWritableFile(const std::string &fname,
                                         WritableFile::Ptr &res,
                                         bool trunc /* = false*/,
                                         bool binary /* = false*/) {
    using namespace std;

    std::string mode = "a";
    if (trunc) mode = "w";
    if (binary) mode += "b";

    FILE *fp = fopen(fname.c_str(), mode.c_str());
    if (!fp) {
        // error msg
        return Status::IOError("Failed to open file");
    }
    WritableFile *writable_file = new details::WritableFileImpl(fp, fname);
    res.reset(writable_file);
    return Status::OK();
}

inline Status FileUtils::ReadAllText(const std::string &fname,
                                     std::string *out) {
    SequentialFile::Ptr seq_file;
    auto status = NewSequentialFile(fname, seq_file, false);
    if (!status.ok()) return status;
    return details::ReadFile(seq_file, out);
}

inline Status FileUtils::ReadAllBytes(const std::string &fname,
                                      std::string *out) {
    SequentialFile::Ptr seq_file;
    auto status = NewSequentialFile(fname, seq_file, true);
    if (!status.ok()) return status;
    return details::ReadFile(seq_file, out);
}

inline Status FileUtils::WriteAllText(const std::string &fname,
                                      const Slice &what) {
    WritableFile::Ptr file;
    auto status = NewWritableFile(fname, file, true, false);
    if (!status.ok()) return status;
    file->append(what);
    return Status::OK();
}

inline Status FileUtils::WriteAllBytes(const std::string &fname,
                                       const Slice &what) {
    WritableFile::Ptr file;
    auto status = NewWritableFile(fname, file, true, true);
    if (!status.ok()) return status;
    file->append(what);
    return Status::OK();
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

inline Status FileUtils::Remove(const std::string &fname) {
    return Remove(fname.c_str());
}

inline Status FileUtils::Remove(const char *fname) {
    int res = std::remove(fname);
    if (res == 0) return Status::OK();
    char buf[64];
    sprintf(buf, "Failed to remove file, res = %d", res);
    return Status::IOError(buf);
}

}  // namespace ijk

#endif
