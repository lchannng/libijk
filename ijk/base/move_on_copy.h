/*
 * File Name: move_on_copy.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2018/10/29 0:34:37
 */

#ifndef _MOVE_ON_COPY_H
#define _MOVE_ON_COPY_H

namespace ijk {

template <typename Moveable>
struct MoveOnCopy {
    mutable Moveable _move_only;

    explicit MoveOnCopy(Moveable &&m) : _move_only(std::move(m)) {}
    MoveOnCopy(MoveOnCopy const &t) : _move_only(std::move(t._move_only)) {}
    MoveOnCopy(MoveOnCopy &&t) : _move_only(std::move(t._move_only)) {}

    MoveOnCopy &operator=(MoveOnCopy const &other) {
        _move_only = std::move(other._move_only);
        return *this;
    }

    MoveOnCopy &operator=(MoveOnCopy &&other) {
        _move_only = std::move(other._move_only);
        return *this;
    }

    Moveable &get() { return _move_only; }

    Moveable release() { return std::move(_move_only); }
};

}  // namespace ijk
#endif
