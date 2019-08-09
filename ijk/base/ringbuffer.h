/*
 * File Name: circular_buffer.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2018/10/7 23:33:20
 */

#ifndef _CIRCULAR_BUFFER_H
#define _RING_BUFFER_H

#include <vector>

namespace ijk {

namespace {
template <typename UnsignedType>
UnsignedType next_pow_of_2(UnsignedType v) {
    static_assert(std::is_unsigned<UnsignedType>::value,
                  "Only works for unsigned types");
    v--;
    for (size_t i = 1; i < sizeof(v) * CHAR_BIT; i *= 2)  // Prefer size_t
                                                          // "Warning comparison
                                                          // between signed and
                                                          // unsigned integer"
    {
        v |= v >> i;
    }
    return ++v;
}
}  // namespace

// FIFO Ring Buffer
template <typename T>
class ring_buffer {
public:
    using ItemType = T;
    explicit ring_buffer(size_t max_items)
        : max_items_(next_pow_of_2(max_items)),
          mask_(max_items_ - 1),
          container_(max_items_) {}

    void clear() {
        wp_ = 0;
        rp_ = 0;
    }

    size_t size() { return wp_ - rp_; }

    void push(ItemType &&item) {
        container_[wp_ & mask_] = std::move(item);
        ++wp_;
    }

    bool pop(ItemType &item) {
        if (rp_ == wp_) return false;
        item = std::move(container_[rp_ & mask_]);
        ++rp_;
        return true;
    }

    bool empty() { return rp_ == wp_; }

    bool full() { return wp_ - rp_ > max_items_; }

    size_t capacity() { return max_items_; };

private:
    size_t max_items_;
    size_t mask_;
    std::vector<ItemType> container_;
    typename std::vector<ItemType>::size_type wp_ = 0;
    typename std::vector<ItemType>::size_type rp_ = 0;
};
}  // namespace ijk
#endif
