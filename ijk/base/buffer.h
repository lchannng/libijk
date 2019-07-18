#pragma once

#ifndef IJK_BUFFER_H
#define IJK_BUFFER_H

#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace ijk {
template <typename ValueType>
class buffer_iterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = ValueType;
    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = std::ptrdiff_t;

    explicit buffer_iterator(pointer p) : _Ptr(p) {}

    reference operator*() const { return *_Ptr; }

    buffer_iterator& operator++() {
        ++_Ptr;
        return *this;
    }

    buffer_iterator operator++(int) {  // postincrement
        buffer_iterator _Tmp = *this;
        ++*this;
        return (_Tmp);
    }

    buffer_iterator& operator--() {
        --_Ptr;
        return *this;
    }

    buffer_iterator operator--(int) {  // postdecrement
        buffer_iterator _Tmp = *this;
        --*this;
        return (_Tmp);
    }

    buffer_iterator& operator+=(
        const difference_type _Off) {  // increment by integer
        _Ptr += _Off;
        return (*this);
    }

    buffer_iterator operator+(
        const difference_type _Off) const {  // return this + integer
        buffer_iterator _Tmp = *this;
        return (_Tmp += _Off);
    }

    buffer_iterator& operator-=(
        const difference_type _Off) {  // decrement by integer
        return (*this += -_Off);
    }

    buffer_iterator operator-(
        const difference_type _Off) const {  // return this - integer
        buffer_iterator _Tmp = *this;
        return (_Tmp -= _Off);
    }

    reference operator[](const difference_type _Off) const {  // subscript
        return (*(*this + _Off));
    }

    difference_type operator-(const buffer_iterator& _Right)
        const {  // return difference of iterators
        return (_Ptr - _Right._Ptr);
    }

    bool operator!=(const buffer_iterator& other) const {
        return _Ptr != other._Ptr;
    }

    bool operator==(
        const buffer_iterator& _Right) const {  // test for iterator equality
        return (_Ptr == _Right._Ptr);
    }

    bool operator<(
        const buffer_iterator& _Right) const {  // test if this < _Right
        return (_Ptr < _Right._Ptr);
    }

    bool operator>(
        const buffer_iterator& _Right) const {  // test if this > _Right
        return (_Right < *this);
    }

    bool operator<=(
        const buffer_iterator& _Right) const {  // test if this <= _Right
        return (!(_Right < *this));
    }

    bool operator>=(
        const buffer_iterator& _Right) const {  // test if this >= _Right
        return (!(*this < _Right));
    }

private:
    pointer _Ptr;
};

template <typename ValueType>
class buffer_const_iterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = ValueType;
    using pointer = const value_type*;
    using reference = const value_type&;
    using difference_type = std::ptrdiff_t;

    explicit buffer_const_iterator(pointer p) : _Ptr(p) {}

    reference operator*() const { return *_Ptr; }

    buffer_const_iterator& operator++() {
        ++_Ptr;
        return *this;
    }

    buffer_const_iterator operator++(int) {  // postincrement
        buffer_const_iterator _Tmp = *this;
        ++*this;
        return (_Tmp);
    }

    buffer_const_iterator& operator--() {
        --_Ptr;
        return *this;
    }

    buffer_const_iterator operator--(int) {  // postdecrement
        buffer_const_iterator _Tmp = *this;
        --*this;
        return (_Tmp);
    }

    buffer_const_iterator& operator+=(
        const difference_type _Off) {  // increment by integer
        _Ptr += _Off;
        return (*this);
    }

    buffer_const_iterator operator+(
        const difference_type _Off) const {  // return this + integer
        buffer_const_iterator _Tmp = *this;
        return (_Tmp += _Off);
    }

    buffer_const_iterator& operator-=(
        const difference_type _Off) {  // decrement by integer
        return (*this += -_Off);
    }

    buffer_const_iterator operator-(
        const difference_type _Off) const {  // return this - integer
        buffer_const_iterator _Tmp = *this;
        return (_Tmp -= _Off);
    }

    reference operator[](const difference_type _Off) const {  // subscript
        return (*(*this + _Off));
    }

    difference_type operator-(const buffer_const_iterator& _Right)
        const {  // return difference of iterators
        return (_Ptr - _Right._Ptr);
    }

    bool operator!=(const buffer_const_iterator& other) const {
        return _Ptr != other._Ptr;
    }

    bool operator==(const buffer_const_iterator& _Right)
        const {  // test for iterator equality
        return (_Ptr == _Right._Ptr);
    }

    bool operator<(
        const buffer_const_iterator& _Right) const {  // test if this < _Right
        return (_Ptr < _Right._Ptr);
    }

    bool operator>(
        const buffer_const_iterator& _Right) const {  // test if this > _Right
        return (_Right < *this);
    }

    bool operator<=(
        const buffer_const_iterator& _Right) const {  // test if this <= _Right
        return (!(_Right < *this));
    }

    bool operator>=(
        const buffer_const_iterator& _Right) const {  // test if this >= _Right
        return (!(*this < _Right));
    }

private:
    pointer _Ptr;
};

class buffer {
public:
    using iterator = buffer_iterator<char>;
    using const_iterator = buffer_const_iterator<char>;
    using value_type = typename iterator::value_type;
    using pointer = typename iterator::pointer;
    using const_pointer = typename const_iterator::pointer;

    // buffer default size
    constexpr static size_t STACK_CAPACITY =
        256 - 4 * sizeof(size_t) - sizeof(std::unique_ptr<value_type[]>);

    enum seek_origin { Begin, Current, End };

    buffer(size_t capacity = STACK_CAPACITY, size_t headreserved = 0)
        : headreserved_(headreserved),
          capacity_(sizeof(stack_data_)),
          readpos_(headreserved),
          writepos_(headreserved),
          stack_data_() {
        if (capacity + headreserved > capacity_) {
            reserve(capacity);
        }
    }

    buffer(const buffer& other) = delete;

    buffer& operator=(const buffer& other) = delete;

    buffer(buffer&&) = default;

    buffer& operator=(buffer&&) = default;

    void init(size_t capacity = STACK_CAPACITY, uint32_t headreserved = 0) {
        readpos_ = headreserved;
        writepos_ = headreserved;
        headreserved_ = headreserved;
        reserve(capacity);
    }

    template <typename T>
    void append(const T* Indata, size_t offset = 0, size_t count = 1) {
        static_assert(std::is_trivially_copyable<T>::value,
                      "type T must be trivially copyable");
        if (nullptr == Indata || 0 == count) return;
        size_t n = sizeof(T) * count;

        reserve(n);

        auto* buff = reinterpret_cast<T*>(std::addressof(*end()));
        memcpy(buff, Indata + offset, n);
        writepos_ += n;
    }

    template <typename T>
    bool prepend(const T* Indata, size_t offset = 0,
                     size_t count = 1) noexcept {
        static_assert(std::is_trivially_copyable<T>::value,
                      "type T must be trivially copyable");
        if (nullptr == Indata || 0 == count || offset >= count) return false;

        size_t n = sizeof(T) * count;

        if (n > readpos_) {
            return false;
        }

        readpos_ -= n;
        auto* buff = reinterpret_cast<T*>(std::addressof(*begin()));
        memcpy(buff, Indata + offset, n);
        return true;
    }

    template <typename T>
    bool read(T* Outdata, size_t offset = 0, size_t count = 1) noexcept {
        static_assert(std::is_trivially_copyable<T>::value,
                      "type T must be trivially copyable");
        if (nullptr == Outdata || 0 == count) return false;

        size_t n = sizeof(T) * count;

        if (readpos_ + n > writepos_) {
            return false;
        }

        auto* buff = std::addressof(*begin());
        memcpy(Outdata + offset, buff, n);
        readpos_ += n;
        return true;
    }

    size_t seek(int offset, seek_origin s = seek_origin::Current) noexcept {
        switch (s) {
            case Begin:
                readpos_ = offset;
                break;
            case Current:
                readpos_ += offset;
                if (readpos_ > writepos_) {
                    readpos_ = writepos_;
                }
                break;
            case End:
                readpos_ = writepos_ + offset;
                if (readpos_ >= writepos_) {
                    readpos_ = writepos_;
                }
                break;
            default:
                break;
        }
        return readpos_;
    }

    void clear() noexcept {
        readpos_ = writepos_ = headreserved_;
    }

    // mark
    void offset_writepos(int offset) noexcept {
        writepos_ += offset;
        if (writepos_ >= capacity_) {
            writepos_ = capacity_;
        }
    }

    const_iterator begin() const noexcept {
        return const_iterator{storage_() + readpos_};
    }

    iterator begin() noexcept { return iterator{storage_() + readpos_}; }

    const_iterator end() const noexcept {
        return const_iterator{storage_() + writepos_};
    }

    iterator end() noexcept { return iterator{storage_() + writepos_}; }

    pointer data() noexcept { return std::addressof(*begin()); }

    const_pointer data() const noexcept { return std::addressof(*begin()); }

    // readable size
    size_t size() const noexcept { return writepos_ - readpos_; }

    size_t capacity() const noexcept { return capacity_; }

    void reserve(size_t need) {
        if (writeable_bytes() >= need) {
            return;
        }

        if (writeable_bytes() + readpos_ < need + headreserved_) {
            auto required_size = writepos_ + need;
            required_size = next_pow2(required_size);
            if (!heap_data_) {
                heap_data_.reset(new char[required_size]);
                memcpy(heap_data_.get(), stack_data_, writepos_);
            } else {
                std::unique_ptr<char[]> new_heap_data(new char[required_size]);
                memcpy(new_heap_data.get(), heap_data_.get(), writepos_);
                heap_data_.swap(new_heap_data);
            }
            capacity_ = required_size;
        } else {
            size_t readable = size();
            if (readable != 0) {
                memcpy(storage_() + headreserved_, storage_() + readpos_, readable);
            }
            readpos_ = headreserved_;
            writepos_ = readpos_ + readable;
        }
    }

    size_t writeable_bytes() const {
        assert(capacity_ >= writepos_);
        return capacity_ - writepos_;
    }

    pointer writable_head() { return storage_() + size(); }

    // commit to writable head
    void commit(size_t nbytes) {
        assert(capacity_ >= writepos_ + nbytes);
        writepos_ += nbytes;
    }

    void consume(size_t nbytes) {
        readpos_ += nbytes;
        if (readpos_ >= writepos_) {
            readpos_ = headreserved_;
            writepos_ = readpos_;
        }
    }

protected:
    size_t next_pow2(size_t x) {
        if (!(x & (x - 1))) {
            return x;
        }
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        return x + 1;
    }

    pointer storage_() noexcept {
        return (!heap_data_) ? stack_data_ : heap_data_.get();
    }

    const_pointer storage_() const noexcept {
        return (!heap_data_) ? stack_data_ : heap_data_.get();
    }

protected:
    size_t headreserved_;

    size_t capacity_;
    // read position
    size_t readpos_;
    // write position
    size_t writepos_;

    std::unique_ptr<value_type[]> heap_data_;

    value_type stack_data_[STACK_CAPACITY];
};

}  // namespace ijk

#endif
