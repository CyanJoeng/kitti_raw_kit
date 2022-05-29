/**
 * Author: Cyan
 * Date: Fri May 27 08:11:02 AM UTC 2022
 */
#include <assert.h>

#include <mutex>
#include <memory>
#include <optional>
#include <condition_variable>

#include "data.hpp"
#include "define.hpp"
#include "image.hpp"
#include "dataset.hpp"

static std::mutex mt;
static std::condition_variable cv;
static kitti_raw::Dataset *_dataset = nullptr;

void image_callback(const std::shared_ptr<kitti_raw::Data> &data) {

    auto &image_data = data->cast<kitti_raw::Image>();

    auto timestamp = data->timestamp();

    printf("image_callback, image timestamp %lf [%s]\n",
            timestamp.toSec(), timestamp.printStr().c_str());

    assert(image_data.isValid());

    {
        std::lock_guard<std::mutex> lck(mt);
        _dataset = nullptr;
    }

    cv.notify_all();
}

int main(int argc, char *argv[]) {

    auto &dataset = kitti_raw::Dataset::Inst();
    _dataset = &dataset;

    _dataset->registerCallback(kitti_raw::IMAGE_00, image_callback);
    _dataset->open();

    std::unique_lock<std::mutex> lck(mt);
    cv.wait(lck, []{
            return _dataset == nullptr;
            });


    return 0;
}
