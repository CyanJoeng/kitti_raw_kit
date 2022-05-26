/**
 * Author: Cyan
 * Date: Wed May 25 07:35:43 AM UTC 2022
 */
#include <assert.h>

#include <map>
#include <string>

#include "define.hpp"

int main(int argc, char ** argv) {

    std::map<std::string, int> count;

    count[kitti_raw::IMAGE_00]++;
    count[kitti_raw::IMAGE_01]++;
    count[kitti_raw::IMAGE_02]++;
    count[kitti_raw::IMAGE_03]++;
    count[kitti_raw::OXTS]++;
    count[kitti_raw::VELODYNE_POINTS]++;

    assert(count.size() == 6);
    assert(count[kitti_raw::IMAGE_00] == 1);
    assert(count[kitti_raw::IMAGE_01] == 1);
    assert(count[kitti_raw::IMAGE_02] == 1);
    assert(count[kitti_raw::IMAGE_03] == 1);
    assert(count[kitti_raw::OXTS] == 1);
    assert(count[kitti_raw::VELODYNE_POINTS] == 1);

    return 0;
}
