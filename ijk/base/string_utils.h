/*
 * File Name: string_utils.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Tue 29 Nov 2016 11:43:35 PM CST
 */

#ifndef _STRING_UTILS_H
#define _STRING_UTILS_H

#include <algorithm>
#include <cstring>
#include <functional>
#include <locale>
#include <string>
#include <vector>

namespace ijk {

template <typename Str>
class StringUtilsT {
public:
    static inline void toUpper(Str *s) {
        std::transform(s->begin(), s->end(), s->begin(), ::toupper);
    }

    static inline void toLower(Str *s) {
        std::transform(s->begin(), s->end(), s->begin(), ::tolower);
    }

    static inline Str toUpper(Str s) {
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        return s;
    }

    static inline Str toLower(Str s) {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    static inline bool startWith(const Str &l, const Str &r) {
        if (r.size() > l.size()) return false;
        return l.compare(0, r.size(), r) == 0;
    }

    static inline bool endWith(const Str &l, const Str &r) {
        if (r.size() > l.size()) return false;
        return std::equal(r.rbegin(), r.rend(), l.rbegin());
    }

    static inline Str &ltrim(Str &s) {
        s.erase(
            s.begin(),
            std::find_if(s.begin(), s.end(), [](typename Str::value_type c) {
                return !std::isspace(c, std::locale::classic());
            }));
        return s;
    }

    static inline Str &rtrim(Str &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(),
                             [](typename Str::value_type c) {
                                 return !std::isspace(c,
                                                      std::locale::classic());
                             })
                    .base(),
                s.end());
        return s;
    }

    static inline void trim(Str *s) { ltrim(rtrim(*s)); }

    static inline Str trim(Str s) {
        ltrim(rtrim(s));
        return s;
    }

    static inline bool contain(const Str &l, const Str &r) {
        return l.find(r) != Str::npos;
    }

    static inline std::vector<Str> tokenize(const Str &s, const Str &delimiters,
                                            bool keep_empty = false) {
        std::vector<Str> tokens;
        typename Str::size_type pos, start = 0;
        while ((pos = s.find_first_of(delimiters, start)) != Str::npos) {
            if (keep_empty || pos != start) {
                tokens.emplace_back(s, start, pos - start);
            }
            start = pos + 1;
        }
        if (keep_empty || start < s.length()) tokens.emplace_back(s, start);
        return tokens;
    }

    template <typename Operator>
    static inline void tokenize(const Str &s, const Str &delimiters,
                                Operator &op, bool keep_empty = false) {
        typename Str::size_type end, start = 0;
        while ((end = s.find_first_of(delimiters, start)) != Str::npos) {
            if (keep_empty || start != end) op(s, start, end);
            start = end + 1;
        }
        end = s.length();
        if (keep_empty || start < end) op(s, start, end);
    }

    static void replace(Str &subject, const Str &search, const Str &replace) {
        typename Str::size_type pos = 0;
        while ((pos = subject.find(search, pos)) != Str::npos) {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
    }
};

using StringUtils = StringUtilsT<std::string>;
using WStringUtils = StringUtilsT<std::wstring>;

}  // namespace ijk

#endif
