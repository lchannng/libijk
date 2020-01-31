/*
 * File Name: snowflake.hpp
 * Author: Stan.Lch
 * Mail: fn.stanc@gmail.com
 * Created Time: 2018/08/25 16:28:02
 */

#ifndef _ID_GENERATOR_H
#define _ID_GENERATOR_H

#include <chrono>
#include <cstdint>
#include <exception>

namespace ijk {

// id = time((63- m -n) bits)+ machine(m bits) + sequence(n bits)
struct default_snowflake_traits {
    constexpr static int64_t start_time = 1577808000000;
    constexpr static int64_t machine_bits = 10;
    constexpr static int64_t sequence_bits = 12;
    static int64_t now() {
        using namespace std::chrono;
        auto duration = system_clock::now().time_since_epoch();
        auto ms = duration_cast<milliseconds>(duration).count();
        return ms;
    }
};

struct snowflake_id_info
{
    int64_t create_time;
    int64_t machine_id;
    int64_t squence;
};

template<typename T = default_snowflake_traits>
class snowflake {
public:
    using traits_type = T;

    snowflake() = default;

    ~snowflake() = default;

    snowflake(const snowflake &) = delete;

    snowflake &operator=(const snowflake &) = delete;


    void init(int machine_id) {
        if (machine_id < 0 || machine_id > kMaxMachineId)
            throw std::runtime_error("bad machine id");
        machine_id_ = machine_id;
    }

    int64_t next_id() {
        int64_t now = traits_type::now();

        if (now < last_timestamp_) {
            // clock is moving backwards
            // last_timestamp_ = now;
            // sequence_ = 0;
            throw std::runtime_error("clock is moving backwards");
        }

        if (last_timestamp_ == now) {
            sequence_ = (sequence_ + 1) & kSequenceMask;
            if (0 == sequence_) {
                now = til_next_time(now);
            }
        } else {
            sequence_ = 0;
        }
        last_timestamp_ = now;
        return ((now - traits_type::start_time) << kTimeShift) | (machine_id_ << kMachineShift) |
               sequence_;
    }

    inline static void get_info(int64_t id, snowflake_id_info &info) {
        info.create_time = ((id >> kTimeShift) & kTimeMask) + traits_type::start_time;
        info.machine_id = (id >> kMachineShift) & kMachineMask;
        info.squence = id & kSequenceMask;
    }

    inline static snowflake_id_info get_info(int64_t id) {
        snowflake_id_info info;
        get_info(id, info);
        return info;
    }

private:
    int64_t til_next_time(int64_t now) {
        int64_t t;
        do {
            t = traits_type::now();
        } while (t <= now);
        return t;
    }

private:
    static constexpr int64_t kMachineBits{traits_type::machine_bits};
    static constexpr int64_t kSequenceBits{traits_type::sequence_bits};
    static constexpr int64_t kTimeBits{63 - kMachineBits - kSequenceBits};

    static constexpr int64_t kMachineShift{kSequenceBits};
    static constexpr int64_t kTimeShift{kMachineBits + kSequenceBits};

    static constexpr int64_t kSequenceMask{(1 << kSequenceBits) - 1};
    static constexpr int64_t kMachineMask{(1 << kMachineBits) - 1};
    static constexpr int64_t kTimeMask{(1LL << kTimeBits) - 1};

    static constexpr int kMaxMachineId{(1 << kMachineBits) - 1};

    int64_t last_timestamp_{0};
    int64_t machine_id_{0};
    int64_t sequence_{0};
};
}  // namespace ijk
#endif
