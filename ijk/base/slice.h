/*
 * File Name: slice.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2017/1/27 12:12:30
 */

#ifndef _SLICE_H
#define _SLICE_H

#include <cassert>
#include <cstring>
#include <string>

namespace ijk {
class Slice {
private:
    const char *data_;
    size_t sz_;

public:
    ~Slice() = default;
    Slice() : data_(""), sz_(0) {}
    Slice(const char *data, size_t sz) : data_(data), sz_(sz) {}
    Slice(const char *s) : data_(s), sz_(strlen(s)) {}
    Slice(const std::string &s) : data_(s.data()), sz_(s.size()) {}
    const char *data() const { return data_; }
    size_t size() const { return sz_; }
    bool empty() const { return 0 == sz_; }

    Slice &operator=(const Slice &rhs) {
        sz_ = rhs.sz_;
        data_ = rhs.data_;
        return *this;
    }

    char operator[](size_t idx) const {
        assert(idx < size());
        return data_[idx];
    }

    void clear() {
        data_ = "";
        sz_ = 0;
    }

    // if *this < s, return -1
    // if *this == s, return 0
    // if *this > s, return 1
    int compare(const Slice &s) const {
        size_t min_len = (sz_ < s.sz_) ? sz_ : s.sz_;
        int r = memcmp(data_, s.data_, min_len);
        if (r == 0) {
            if (sz_ < s.sz_)
                r = -1;
            else if (sz_ > s.sz_)
                r = +1;
        }
        return r;
    }

    bool operator==(const Slice &rhs) const {
        return (sz_ == rhs.sz_) && (memcmp(data_, rhs.data_, sz_) == 0);
    }
};
}  // namespace ijk
#endif
