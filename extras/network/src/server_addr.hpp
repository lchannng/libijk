/*
 * File  : server_ident.hpp
 * Author: Lch <l.channng@gmail.com>
 * Date  : 2019/08/20 19:37:15
 */

#pragma once
#ifndef SERVER_IDENT_HPP_UBCOU1ML
#define SERVER_IDENT_HPP_UBCOU1ML

#include "ijk/base/fmt.h"

#include <cstdint>
#include <string>

namespace xx {

// server address like ip address, 172.0.0.1
class server_addr final {
public:
    server_addr() : svr_id(0) {}
    server_addr(uint32_t id) : svr_id(id) {}

    server_addr(uint8_t c_id, uint8_t z_id, uint8_t t_id, uint8_t i_id)
        : channel_id(c_id), zone_id(z_id), type_id(t_id), inst_id(i_id) {}

    bool is_null() const { return 0 == svr_id; }

    std::string to_string() const {
        return fmt::format("{}.{}.{}.{}", channel_id, zone_id, type_id,
                           inst_id);
    }

    bool operator<(const server_addr &rhs) const { return svr_id < rhs.svr_id; }
    bool operator==(const server_addr &rhs) const { return svr_id == rhs.svr_id; }
    bool operator!=(const server_addr &rhs) const { return svr_id != rhs.svr_id; }

    union {
        uint32_t svr_id;
        struct {
            uint8_t channel_id;
            uint8_t zone_id;
            uint8_t type_id;
            uint8_t inst_id;
        };
    };
};

}  // namespace xx

#endif /* end of include guard: SERVER_IDENT_HPP_UBCOU1ML */
