/**
 * Author: Cyan
 * Date: Thu May 26 10:19:01 AM UTC 2022
 */
#pragma once
#include <string>

namespace kitti_raw {

    class Timestamp {

    public:
        explicit Timestamp(const std::string &time_str);

        explicit Timestamp(u_int64_t sec, u_int64_t usec);

        auto toSec() const -> double;

        operator double() const {
            return this->toSec();
        }

        auto printStr() const -> std::string;

        auto operator== (const Timestamp& inst) const -> bool;

    private:
        uint64_t second_ {0};
        uint64_t u_second_ {0};
    };
}
