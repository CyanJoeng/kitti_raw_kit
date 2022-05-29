/**
 * Author: Cyan
 * Date: Fri May 27 02:47:43 AM UTC 2022
 */
#pragma once
#include <vector>

#include "data.hpp"

namespace kitti_raw {

    class Image : public Data {

    public:
        Image(const Timestamp &timestamp);

        Image(const Timestamp &timestamp, std::vector<char> data);

        virtual auto setData(const std::vector<char> &data) -> bool;

        virtual auto isValid() const -> bool override;

    private:
        std::vector<char> raw_data_;
    };
}
