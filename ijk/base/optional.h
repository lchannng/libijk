//
// Copyright (c) 2014-2018 Martin Moene
//
// https://github.com/martinmoene/optional-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef _IJK_OPTIONAL_H
#define _IJK_OPTIONAL_H

#include "details/optional.h"

namespace ijk {

template <typename T>
using Optional = nonstd::optional<T>;

}  // namespace ijk

#define Nullopt nonstd::nullopt;

#endif  // _IJK_OPTIONAL_H
