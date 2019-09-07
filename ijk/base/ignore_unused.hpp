/*
 * File  : ignore_unused.hpp
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/09/07 17:59:19
 */

#pragma once
#ifndef IGNORE_UNUSED_HPP_C1LVFJU5
#define IGNORE_UNUSED_HPP_C1LVFJU5

namespace ijk {
template <typename... T>
inline void ignore_unused(T &&...) {}
}  // namespace ijk

#endif /* end of include guard: IGNORE_UNUSED_HPP_C1LVFJU5 */
