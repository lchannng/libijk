/*
 * File Name: id_generator.h
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
class IdGenerator {
    // Id = time(41bits)+ machine(10bits) + sequence(12bits)
public:
    IdGenerator(int machine_id) {
        if (machine_id > kMaxMachineId)
            throw std::runtime_error("bad machine id");
        machine_id_ = machine_id;
    }

    int64_t nextId() {
        int64_t now = timeGen();

        if (now < last_timestamp_) {
            // clock is moving backwards
            last_timestamp_ = now;
            sequence_ = 0;
        }

        if (last_timestamp_ == now) {
            sequence_ = (sequence_ + 1) & kSequenceMask;
            if (0 == sequence_) {
                now = tilNextMillis(now);
            }
        } else {
            sequence_ = 0;
        }
        last_timestamp_ = now;
        return ((now - kEpoch) << kTimeShift) | (machine_id_ << kMachineShift) |
               sequence_;
    }

    static void getInfo(int64_t id, int64_t *create_time, int *machine_id,
                        int *seq) {
        if (create_time)
            *create_time = ((id >> kTimeShift) & kTimeMask) + kEpoch;
        if (machine_id) *machine_id = (id >> kMachineShift) & kMachineMask;
        if (seq) *seq = id & kSequenceMask;
    }

private:
    int64_t timeGen() {
        using namespace std::chrono;
        auto duration = system_clock::now().time_since_epoch();
        auto ms = duration_cast<milliseconds>(duration).count();
        return ms;
    }

    int64_t tilNextMillis(int64_t now) {
        int64_t t;
        do {
            t = timeGen();
        } while (t <= now);
        return t;
    }

private:
    static const int64_t kEpoch{1288834974657};

    static const int kMachineBits{10};
    static const int kSequenceBits{12};
    static const int kTimeBits{41};

    static const int kMachineShift{kSequenceBits};
    static const int kTimeShift{kMachineBits + kSequenceBits};

    static const int kSequenceMask{-1 ^ (-1 << kSequenceBits)};
    static const int kMachineMask{-1 ^ (-1 << kMachineBits)};
    static const int64_t kTimeMask{(-1LL ^ (-1LL << kTimeBits))};

    static const int kMaxMachineId{-1 ^ (-1 << kMachineBits)};

    int64_t last_timestamp_{0};
    int machine_id_{0};
    int sequence_{0};
};
}  // namespace ijk
#endif
