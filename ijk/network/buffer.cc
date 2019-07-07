/*
 * File Name: buffer.cpp
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Mon 22 Aug 2016 11:30:42 PM CST
 */

#include "buffer.h"
#include <algorithm>
#include <cassert>
#include <cstring>

namespace ijk {

Buffer::Buffer(size_t min_headroom)
    : storage_(min_headroom),
      in_(min_headroom),
      out_(min_headroom),
      min_headroom_(min_headroom) {
    storage_.resize(min_headroom);
}

Buffer::~Buffer() {}

const uint8_t *Buffer::data() const {
    return storageBegin() + out_;
}
uint8_t *Buffer::data() {
    return storageBegin() + out_;
}

size_t Buffer::size() const {
    assert(in_ >= out_);
    return in_ - out_;
}

bool Buffer::empty() const {
    return size() == 0;
}

const uint8_t *Buffer::tail() const {
    return storageBegin() + in_;
}

uint8_t *Buffer::tail() {
    return storageBegin() + in_;
}

size_t Buffer::tailRoom() const {
    return storageEnd() - tail();
}

size_t Buffer::headRoom() const {
    return out_;
}

void Buffer::clear() {
    in_ = out_ = min_headroom_;
}

void Buffer::swap(Buffer &other) {
    std::swap(storage_, other.storage_);
    std::swap(in_, other.in_);
    std::swap(out_, other.out_);
}

void Buffer::append(const void *buf, size_t sz) {
    if (0 == sz) return;
    reserve(sz);
    memcpy(tail(), buf, sz);
    in_ += sz;
}

void Buffer::commit(size_t sz) {
    in_ += sz;
    assert(in_ <= storage_.size());
}

bool Buffer::prepend(const void *buf, size_t sz) {
    if (sz > out_) return false;
    uint8_t *d = data() - sz;
    memcpy(d, buf, sz);
    out_ -= sz;
    return true;
}

uint8_t *Buffer::tryPrepend(size_t sz) {
    if (sz > out_) return nullptr;
    uint8_t *d = data() - sz;
    return d;
}

bool Buffer::prepend(size_t sz) {
    if (sz > out_) {
        return false;
    }
    out_ -= sz;
    return true;
}

void Buffer::drain(size_t sz) {
    out_ += sz;
    if (out_ >= in_) {
        out_ = in_ = min_headroom_;
    }
}

void Buffer::drainAll() {
    drain(size());
}

void Buffer::shrink(size_t reserve) {
    Buffer new_buf(min_headroom_);
    new_buf.reserve(size() + reserve);
    new_buf.append(data(), size());
    swap(new_buf);
}

const uint8_t *Buffer::peek(size_t peek_size, size_t offset /*= 0*/) {
    if (size() < (peek_size + offset)) {
        return nullptr;
    }
    return data() + offset;
}

const uint8_t *Buffer::storageBegin() const {
    return const_cast<Buffer *>(this)->storageBegin();
}

uint8_t *Buffer::storageBegin() {
    if (storage_.empty()) return nullptr;
    return &*storage_.begin();
}

const uint8_t *Buffer::storageEnd() const {
    return const_cast<Buffer *>(this)->storageEnd();
}

uint8_t *Buffer::storageEnd() {
    if (storage_.empty()) return nullptr;
    return &storage_.back() + 1;
}

void Buffer::reserve(size_t sz) {
    if (tailRoom() >= sz) return;

    /*
     *      sz
     *     =======
     * ===|===========|==============|====
     * min_headroom   out           in
     */
    if ((out_ - min_headroom_) >= sz) {
        size_t curr_sz = size();
        std::memmove(storageBegin() + min_headroom_, storageBegin() + out_,
                     curr_sz);
        out_ = min_headroom_;
        in_ = out_ + curr_sz;
        return;
    }

    /*
     *           sz
     * ======================
     * ==============|==============|====
     *              out           in
     */
    storage_.resize(sz + in_);
}

size_t Buffer::capacity() const {
    return storage_.size();
}

}  // namespace ijk
