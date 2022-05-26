/**
 * Author: Cyan
 * Date: Thu May 26 10:22:25 AM UTC 2022
 */
#include "timestamp.hpp"
#include <ctime>
#include <iomanip>
#include <locale>
#include <charconv>
#include <sstream>

namespace kitti_raw {

    /**
     * time_str:   2011-09-26 13:02:37.004854985
     * format_str: %Y-%m-%d %H:%M:%S.xxxms
     */
    Timestamp::Timestamp(const std::string &time_str) {

        auto st_point = time_str.rfind('.');

        std::tm t = {};
        std::istringstream ss(time_str.substr(0, st_point));
        ss.imbue(std::locale("en_US.UTF8"));
        ss >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");

        if (ss.fail()) {

            std::stringstream ss;
            ss << "Timestamp parse error: " << time_str << ' ' << time_str.substr(0, st_point);
            throw std::runtime_error(ss.str());
        }

        std::string n_sec_str = time_str.substr(st_point + 1);

        this->second_ = std::mktime(&t);
        std::sscanf(n_sec_str.c_str(), "%lu", &this->u_second_);
    }

    Timestamp::Timestamp(u_int64_t sec, u_int64_t usec) :
        second_(sec), u_second_(usec) {}

    auto Timestamp::toSec() const -> double {

        return this->second_ + this->u_second_ * 1e-9;
    }

    auto Timestamp::operator==(const Timestamp &inst) const -> bool {

        return this->second_ == inst.second_ && this->u_second_ == inst.u_second_;
    }
}
