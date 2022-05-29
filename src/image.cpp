/**
 * Author: Cyan
 * Date: Fri May 27 03:25:36 AM UTC 2022
 */
#include "image.hpp"

namespace kitti_raw {

    Image::Image(const Timestamp &timestamp) : Data(timestamp) {}

    Image::Image(const Timestamp &timestamp, std::vector<char> data) :
        Data(timestamp), raw_data_(std::move(data)) {}

    auto Image::setData(const std::vector<char> &data) -> bool {

        if (data.empty()) {

            printf("Image setData, set image data by empty data");
            return false;
        }

        this->raw_data_ = data;

        return true;
    }

    auto Image::isValid() const -> bool {

        return ! this->raw_data_.empty();
    }
}
