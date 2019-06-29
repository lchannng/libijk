/*
 * File  : endian.h
 * Author: Lch <fn.stanc@gmail.com>
 * Date  : 2018/12/13 19:44:21
 */

#ifndef _IJK_ENDIAN_H
#define _IJK_ENDIAN_H

#include <cstddef>
#include <type_traits>

namespace ijk {

namespace details {

template <typename T, size_t SIZE>
struct IsInteger {
    enum {
        Value = sizeof(T) == SIZE && std::is_integral<T>::value,
    };
};

template <typename T>
inline constexpr typename std::enable_if<IsInteger<T, 1>::Value, T>::type
swapBytes(T value) noexcept {
    return value;
}

template <typename T>
inline constexpr typename std::enable_if<IsInteger<T, 2>::Value, T>::type
swapBytes(T value) noexcept {
    return ((value & 0x00FF) << 8) | ((value & 0xFF00) >> 8);
}

template <typename T>
inline constexpr typename std::enable_if<IsInteger<T, 4>::Value, T>::type
swapBytes(T value) noexcept {
    return ((value & 0x000000FF) << 24) | ((value & 0x0000FF00) << 8) |
           ((value & 0x00FF0000) >> 8) | ((value & 0xFF000000) >> 24);
}

template <typename T>
inline constexpr typename std::enable_if<IsInteger<T, 8>::Value, T>::type
swapBytes(T value) noexcept {
    return ((value & 0xFF00000000000000ull) >> 56) |
           ((value & 0x00FF000000000000ull) >> 40) |
           ((value & 0x0000FF0000000000ull) >> 24) |
           ((value & 0x000000FF00000000ull) >> 8) |
           ((value & 0x00000000FF000000ull) << 8) |
           ((value & 0x0000000000FF0000ull) << 24) |
           ((value & 0x000000000000FF00ull) << 40) |
           ((value & 0x00000000000000FFull) << 56);
}

}  // namespace details

}  // namespace ijk

#ifndef IJK_BIG_ENDIAN

#ifdef _BIG_ENDIAN_
#if _BIG_ENDIAN_
#define IJK_BIG_ENDIAN 1
#endif
#endif

#ifndef IJK_BIG_ENDIAN
#if defined(__hppa__) || defined(__m68k__) || defined(mc68000) ||         \
    defined(_M_M68K) || (defined(__MIPS__) && defined(__MIPSEB__)) ||     \
    defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) ||        \
    defined(__sparc__) || defined(__powerpc__) || defined(__mc68000__) || \
    defined(__s390x__) || defined(__s390__)
#define IJK_BIG_ENDIAN 1
#endif
#endif

#ifndef IJK_BIG_ENDIAN
#define IJK_BIG_ENDIAN 0
#endif

#endif

#if IJK_BIG_ENDIAN

#ifndef htole
#define htole(x) ijk::details::swapBytes(x)
#endif  // !htole

#ifndef letoh
#define letoh(x) ijk::details::swapBytes(x)
#endif  // !letoh

#ifndef htobe
#define htobe(x) (x)
#endif  // !htobe

#ifndef betoh
#define betoh(x) (x)
#endif  // !betoh

#else

#ifndef htole
#define htole(x) (x)
#endif  // !htole

#ifndef letoh
#define letoh(x) (x)
#endif  // !letoh

#ifndef htobe
#define htobe(x) ijk::details::swapBytes(x)
#endif  // !htobe

#ifndef betoh
#define betoh(x) ijk::details::swapBytes(x)
#endif  // !betoh

#endif

#endif /* ifndef _IJK_ENDIAN_H*/
