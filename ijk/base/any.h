#pragma once
#ifndef INC_PM_ANY_HPP_
#define INC_PM_ANY_HPP_

/*
 * Promise API implemented by cpp as Javascript promise style
 *
 * Copyright (c) 2016, xhawk18
 * at gmail.com
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <typeindex>

namespace ijk {

// Any library
// See http://www.boost.org/libs/any for Documentation.
// what:  variant type any
// who:   contributed by Kevlin Henney,
//        with features contributed and bugs found by
//        Ed Brey, Mark Rodgers, Peter Dimov, and James Curran
// when:  July 2001
// where: tested with BCC 5.5, MSVC 6.0, and g++ 2.95

class Any {
public:  // structors
    Any() : content(0) {}

    template <typename ValueType>
    Any(const ValueType &value) : content(new holder<ValueType>(value)) {}

    Any(const Any &other)
        : content(other.content ? other.content->clone() : 0) {}

    Any(Any &&other) : content(other.content) { other.content = 0; }

    template <typename ValueType>
    Any(ValueType &&value,
        typename std::enable_if<!std::is_same<Any &, ValueType>::value>::type
            * = 0  // disable if value has type `any&`
        ,
        typename std::enable_if<!std::is_const<ValueType>::value>::type * =
            0)  // disable if value has type `const ValueType&&`
        : content(new holder<typename std::decay<ValueType>::type>(
              static_cast<ValueType &&>(value))) {}

    ~Any() {
        if (content != nullptr) {
            delete content;
        }
    }

public:  // modifiers
    Any &swap(Any &rhs) {
        std::swap(content, rhs.content);
        return *this;
    }

    Any &operator=(const Any &rhs) {
        Any(rhs).swap(*this);
        return *this;
    }

    // move assignement
    Any &operator=(Any &&rhs) {
        rhs.swap(*this);
        Any().swap(rhs);
        return *this;
    }

    // Perfect forwarding of ValueType
    template <class ValueType>
    Any &operator=(ValueType &&rhs) {
        Any(static_cast<ValueType &&>(rhs)).swap(*this);
        return *this;
    }

public:  // queries
    bool empty() const { return !content; }

    void clear() { Any().swap(*this); }

    const std::type_info &type() const {
        return content ? content->type() : typeid(void);
    }

public:  // types (public so any_cast can be non-friend)
    class placeholder {
    public:  // structors
        virtual ~placeholder() {}

    public:  // queries
        virtual const std::type_info &type() const = 0;
        virtual placeholder *clone() const = 0;
    };

    template <typename ValueType>
    class holder : public placeholder {
    public:  // structors
        holder(const ValueType &value) : held(value) {}

        holder(ValueType &&value) : held(static_cast<ValueType &&>(value)) {}

    public:  // queries
        virtual const std::type_info &type() const { return typeid(ValueType); }
        virtual placeholder *clone() const { return new holder(held); }

    public:  // representation
        ValueType held;

    private:  // intentionally left unimplemented
        holder &operator=(const holder &);
    };

public:  // representation (public so any_cast can be non-friend)
    placeholder *content;
};

class bad_any_cast : public std::bad_cast {
public:
    std::type_index from_;
    std::type_index to_;
    bad_any_cast(const std::type_index &from, const std::type_index &to)
        : from_(from), to_(to) {}
    virtual const char *what() const throw() { return "bad_any_cast"; }
};

template <typename ValueType>
ValueType *AnyCast(Any *operand) {
    typedef typename Any::template holder<ValueType> holder_t;
    return operand && operand->type() == typeid(ValueType)
               ? &static_cast<holder_t *>(operand->content)->held
               : 0;
}

template <typename ValueType>
inline const ValueType *AnyCast(const Any *operand) {
    return AnyCast<ValueType>(const_cast<Any *>(operand));
}

template <typename ValueType>
ValueType AnyCast(Any &operand) {
    typedef typename std::remove_reference<ValueType>::type nonref;

    nonref *result = AnyCast<nonref>(&operand);
    if (!result)
        throw(bad_any_cast(std::type_index(operand.type()),
                           std::type_index(typeid(ValueType))));
    return *result;
}

template <typename ValueType>
inline ValueType AnyCast(const Any &operand) {
    typedef typename std::remove_reference<ValueType>::type nonref;
    return AnyCast<const nonref &>(const_cast<Any &>(operand));
}

// Copyright Kevlin Henney, 2000, 2001, 2002. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

}  // namespace ijk
#endif
