/**
 * Author: Cyan
 * Date: Fri May 27 02:53:07 AM UTC 2022
 */
#include "data.hpp"

namespace kitti_raw {

    Data::Data(const Timestamp &timestamp) :
        timestamp_(timestamp) {}

    auto Data::timestamp() const -> const Timestamp& {

        return this->timestamp_;
    }
}
