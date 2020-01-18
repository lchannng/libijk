/*
 * File Name: string_utils.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Tue 29 Nov 2016 11:43:35 PM CST
 */

#ifndef _STRING_UTILS_H
#define _STRING_UTILS_H

#include <algorithm>
#include <cctype>
#include <charconv>
#include <cmath>
#include <iomanip>
#include <locale>
#include <sstream>
#include <vector>

namespace ijk {

class string_utils final {
public:
    using string_view_t = std::string_view;

    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    static inline T string_convert(const string_view_t &s, std::errc &ec) {
        T result;
        auto res = std::from_chars(s.data(), s.data() + s.size(), result);
        ec = res.ec;
        return result;
    }

    template <typename T, std::enable_if_t<std::is_arithmetic_v<T>, int> = 0>
    static inline T string_convert(const string_view_t &s) {
        T result;
        if (auto [p, ec] = std::from_chars(s.data(), s.data() + s.size(), result); ec != std::errc()) {
            throw std::runtime_error(std::to_string(static_cast<int>(ec)));
        }
        return result;
    }

    template <typename T, std::enable_if_t<std::is_same_v<T, std::string>, int> = 0>
    static inline std::string string_convert(const string_view_t &s) {
        return std::string(s);
    }

    template <typename T, std::enable_if_t<std::is_same_v<T, string_view_t>, int> = 0>
    static inline string_view_t string_convert(const string_view_t &s) {
        return s;
    }

    /*
    e. split("aa/bb/cc","/")
    */
    template <class T>
    static std::vector<T> split(const string_view_t &src, const string_view_t &sep) {
        std::vector<T> r;
        string_view_t::const_iterator b = src.begin();
        string_view_t::const_iterator e = src.end();
        for (auto i = src.begin(); i != src.end(); ++i) {
            if (sep.find(*i) != string_view_t::npos) {
                if (b != e && b != i) {
                    r.push_back(string_convert<T>(string_view_t(std::addressof(*b), size_t(i - b))));
                }
                b = e;
            } else {
                if (b == e) {
                    b = i;
                }
            }
        }
        if (b != e) r.push_back(string_convert<T>(string_view_t(std::addressof(*b), size_t(e - b))));
        return r;
    }

    //format string
    static inline std::string format(const char *fmt, ...) {
        if (!fmt) return std::string("");

        static constexpr size_t MAX_FMT_LEN = 8192;
        static thread_local char fmtbuf[MAX_FMT_LEN + 1];
        va_list ap;
        va_start(ap, fmt);
        // win32
#if TARGET_PLATFORM == PLATFORM_WINDOWS
        int n = vsnprintf_s(fmtbuf, MAX_FMT_LEN, fmt, ap);
#else
        int n = vsnprintf(fmtbuf, MAX_FMT_LEN, fmt, ap);
#endif
        va_end(ap);
        return std::string(fmtbuf, n);
    }

    //return left n char
    static inline std::string left(const std::string &str, size_t n) {
        return std::string(str, 0, n);
    }

    //return right n char
    static inline std::string right(const std::string &str, size_t n) {
        size_t s = (str.size() >= n) ? (str.size() - n) : 0;
        return std::string(str, s);
    }

    //" /t/n/r"
    static inline string_view_t trim_right(string_view_t v) {
        const auto words_end(v.find_last_not_of(" \t\n\r"));
        if (words_end != string_view_t::npos) {
            v.remove_suffix(v.size() - words_end - 1);
        }
        return v;
    }

    static inline string_view_t trim_left(string_view_t v) {
        const auto words_begin(v.find_first_not_of(" \t\n\r"));
        v.remove_prefix(std::min(words_begin, v.size()));
        return v;
    }

    static inline string_view_t trim_surrounding(string_view_t v) {
        const auto words_end(v.find_last_not_of(" \t\n\r"));
        if (words_end != string_view_t::npos) {
            v.remove_suffix(v.size() - words_end - 1);
        }
        const auto words_begin(v.find_first_not_of(" \t\n\r"));
        v.remove_prefix(std::min(words_begin, v.size()));
        return v;
    }

    static inline void replace(std::string &src, string_view_t old, string_view_t strnew) {
        for (std::string::size_type pos(0); pos != std::string::npos; pos += strnew.size()) {
            if ((pos = src.find(old, pos)) != std::string::npos)
                src.replace(pos, old.size(), strnew);
            else
                break;
        }
    }

    //https://en.cppreference.com/w/cpp/string/byte/tolower
    //the behavior of std::tolower is undefined if the argument's value is neither representable
    //as unsigned char nor equal to EOF.
    //To use these functions safely with plain chars (or signed chars),
    //	the argument should first be converted to unsigned char
    static inline char toupper(unsigned char c) {
        return static_cast<char>(std::toupper(c));
    }

    static inline char tolower(unsigned char c) {
        return static_cast<char>(std::tolower(c));
    }

    static inline void upper(std::string &src) {
        std::transform(src.begin(), src.end(), src.begin(), toupper);
    }

    static inline void lower(std::string &src) {
        std::transform(src.begin(), src.end(), src.begin(), tolower);
    }

    static inline bool starts_with(string_view_t s, string_view_t prefix) {
        if (prefix.size() > s.size()) return false;
        return std::equal(s.begin(), s.end(), prefix.begin());
    }

    static inline bool ends_with(string_view_t s, string_view_t suffix) {
        if (suffix.size() > s.size()) return false;
        return std::equal(s.rbegin(), s.rend(), suffix.rbegin());
    }

    template <typename TString>
    static inline bool iequal_string(const TString &str1, const TString &str2) {
        if (str1.size() != str2.size())
            return false;

        auto iter1begin = str1.begin();
        auto iter2begin = str2.begin();

        auto iter1end = str1.end();
        auto iter2end = str2.end();

        for (; iter1begin != iter1end && iter2begin != iter2end; ++iter1begin, ++iter2begin) {
            if (!(toupper(*iter1begin) == toupper(*iter2begin))) {
                return false;
            }
        }
        return true;
    }

    static inline std::string hex_string(string_view_t s, string_view_t tok = "") {
        std::stringstream ss;
        ss << std::setiosflags(std::ios::uppercase) << std::hex;
        for (auto c : s) {
            ss << (int)(uint8_t)(c) << tok;
        }
        return ss.str();
    }
};

}  // namespace ijk

#endif
