/**
 * Author: Cyan
 * Date: Thu May 26 11:54:10 AM UTC 2022
 */
#include <assert.h>
#include <exception>
#include <fstream>

#include "timestamp.hpp"


int main(int argc, char *argv[]) {

    assert(argc == 3);

    u_int64_t sec, usec;
    sscanf(argv[2], "%lu.%lu", &sec, &usec);

    try {

        kitti_raw::Timestamp t(argv[1]);
        kitti_raw::Timestamp t2(sec, usec);
        assert(t == t2);
    } catch (const std::exception &e) {

        assert(false);
    }

    return 0;
}
