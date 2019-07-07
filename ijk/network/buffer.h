/*
 * File Name: buffer.h
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: Mon 22 Aug 2016 03:26:35 PM GMT
 */

#ifndef _BUFFER_H
#define _BUFFER_H

#include "ijk/base/endian.h"
#include "ijk/base/gsl.h"

//#include <stddef.h>
#include <cstdint>
#include <vector>

namespace ijk {

/*
 *  +--------+
 *  | Buffer |
 *  +--------+
 *   /
 *  |			 out_	 		     in_
 *  v			 v		 size()		  v
 *  +------------+--------------------+-----------+
 *  | headroom   |        data        |  tailroom |
 *  +------------+--------------------+-----------+
 *  ^            ^                    ^           ^
 * begin()     data()               tail()     end()
 */

class Buffer final {
public:
    explicit Buffer(size_t min_headroom = 0);
    ~Buffer();
    const uint8_t *data() const;
    uint8_t *data();
    size_t size() const;
    bool empty() const;
    size_t capacity() const;
    uint8_t *tail();
    size_t tailRoom() const;
    void reserve(size_t sz);
    size_t headRoom() const;
    void clear();
    void swap(Buffer &other);
    void append(const void *buf, size_t sz);
    void commit(size_t sz);

    bool prepend(const void *buf, size_t sz);
    uint8_t *tryPrepend(size_t sz);
    bool prepend(size_t sz);

    void drain(size_t sz);
    void drainAll();
    void shrink(size_t reserve);

    /*!
     * \brief Peek data at Data() + offset, if remaining size < peek_size,
     * return nullptr
     *
     * \param peek_size
     * \param offset
     * \return
     */
    const uint8_t *peek(size_t peek_size, size_t offset);

    /*!
     * \brief Encode an integer with little endian, and append to buffer
     *
     * \param val
     */
    template <typename T, typename = typename std::enable_if<
                              std::is_integral<T>::value>::type>
    void append(T val) {
        val = htole(val);
        append(&val, sizeof(val));
    }

    /*!
     * \brief Read an integer from buffer
     *
     * \param val
     * \return
     */
    template <typename T, typename = typename std::enable_if<
                              std::is_integral<T>::value>::type>
    T read() {
        T val = peek<T>(0);
        drain(sizeof(T));
        return val;
    }

    /*!
     * \brief Peek an integer at Data() + offset
     *
     * \param val
     * \param offset
     * \return
     */
    template <typename T, typename = typename std::enable_if<
                              std::is_integral<T>::value>::type>
    T peek(size_t offset) {
        Expects(size() >= sizeof(T));
        T val = letoh(*(T *)(data() + offset));
        return val;
    }

private:
    const uint8_t *tail() const;
    const uint8_t *storageBegin() const;
    uint8_t *storageBegin();
    const uint8_t *storageEnd() const;
    uint8_t *storageEnd();

private:
    std::vector<uint8_t> storage_;
    size_t in_;
    size_t out_;
    size_t min_headroom_;
};

}  // namespace ijk

#endif
