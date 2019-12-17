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

// Id = time((63- m -n) bits)+ machine(m bits) + sequence(n bits)
template<int MachineBits = 10, int SequenceBits = 12>
class snowflake {
public:
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
        int64_t now = now_ms();

        if (now < last_timestamp_) {
            // clock is moving backwards
            // last_timestamp_ = now;
            // sequence_ = 0;
            throw std::runtime_error("clock is moving backwards");
        }

        if (last_timestamp_ == now) {
            sequence_ = (sequence_ + 1) & kSequenceMask;
            if (0 == sequence_) {
                now = til_next_millis(now);
            }
        } else {
            sequence_ = 0;
        }
        last_timestamp_ = now;
        return ((now - kEpoch) << kTimeShift) | (machine_id_ << kMachineShift) |
               sequence_;
    }

    static void get_info(int64_t id, int64_t *create_time, int *machine_id,
                        int *seq) {
        if (create_time)
            *create_time = ((id >> kTimeShift) & kTimeMask) + kEpoch;
        if (machine_id) *machine_id = (id >> kMachineShift) & kMachineMask;
        if (seq) *seq = id & kSequenceMask;
    }

private:
    int64_t now_ms() {
        using namespace std::chrono;
        auto duration = system_clock::now().time_since_epoch();
        auto ms = duration_cast<milliseconds>(duration).count();
        return ms;
    }

    int64_t til_next_millis(int64_t now) {
        int64_t t;
        do {
            t = now_ms();
        } while (t <= now);
        return t;
    }

private:
    static constexpr int64_t kEpoch{1576497960000};

    static constexpr int64_t kMachineBits{MachineBits};
    static constexpr int64_t kSequenceBits{SequenceBits};
    static constexpr int64_t kTimeBits{63 - kMachineBits - kSequenceBits};

    static constexpr int64_t kMachineShift{kSequenceBits};
    static constexpr int64_t kTimeShift{kMachineBits + kSequenceBits};

    static constexpr int64_t kSequenceMask{-1 ^ (-1 << kSequenceBits)};
    static constexpr int64_t kMachineMask{-1 ^ (-1 << kMachineBits)};
    static constexpr int64_t kTimeMask{(-1LL ^ (-1LL << kTimeBits))};

    static constexpr int kMaxMachineId{-1 ^ (-1 << kMachineBits)};

    int64_t last_timestamp_{0};
    int64_t machine_id_{0};
    int64_t sequence_{0};
};
}  // namespace ijk
#endif
