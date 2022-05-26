/**
 * Author: Cyan
 * Date: Tue May 24 03:27:45 AM UTC 2022
 */
#include "kitti_kit_raw.hpp"

#include "dataset.hpp"

auto KittiRaw::Start(const std::string &config_file) -> bool {

    if (!kitti_raw::Dataset::Inst().open(config_file)) {

        printf("Config can not be opened\n");
        return false;
    }

    return true;
}
