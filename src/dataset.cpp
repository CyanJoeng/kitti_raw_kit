/**
 * Author: Cyan
 * Date: Tue May 24 03:38:34 AM UTC 2022
 */
#include "dataset.hpp"

#include <assert.h>

#include <cstdio>
#include <memory>
#include <mutex>
#include <vector>
#include <fstream>
#include <exception>
#include <filesystem>

#include "config.hpp"
#include "define.hpp"
#include "image.hpp"
#include "timestamp.hpp"


namespace kitti_raw {

    auto Dataset::Inst() -> Dataset& {

        static Dataset inst;
        return inst;
    }

    Dataset::Dataset() {

        for (auto imge_label : {IMAGE_00, IMAGE_01, IMAGE_02, IMAGE_03}) {

            this->data_pool_[imge_label].job_read = std::bind(&Dataset::readImage, this, imge_label);
        }

        this->data_pool_[OXTS].job_read = std::bind(&Dataset::readOxts, this);
    }

    Dataset::~Dataset() {

        this->close();

        for (auto &[label, data_bundle] : this->data_pool_) {

            std::lock_guard<std::mutex> lck(data_bundle.mt);

            data_bundle.data_list.clear();
            data_bundle.cv.notify_all();

            if (data_bundle.post_process.joinable()) {
                data_bundle.post_process.join();
            }
        }
    }

    auto Dataset::open(const std::string &config_file) -> bool {

        if (config_file.empty()) {

            this->config_ = Config::Default();
        } else {

            try {
                this->config_ = Config::LoadFromFile(config_file);
            } catch (const std::exception &e) {

                return false;
            }
        }

        try {
            this->config_ = config_file.empty()? Config::Default(): Config::LoadFromFile(config_file);
        } catch (const std::exception &e) {

            return false;
        }

        if (!this->startStreams()) {

            printf("Dataset: open, start streams fail\n");
            return false;
        }

        return true;
    }

    auto Dataset::registerCallback(const std::string &label, const Callback &callback) -> bool {

        auto data_process = [this](const std::string &label) {

            auto &data_bundle = this->data_pool_[label];
            while (!data_bundle.exit || !data_bundle.data_list.empty()) {

                std::shared_ptr<Data> data = nullptr;
                {
                    std::unique_lock<std::mutex> lck(data_bundle.mt);
                    data_bundle.cv.wait(lck, [&data_bundle]{
                            return data_bundle.exit || (!data_bundle.data_list.empty());
                            });

                    if (!data_bundle.data_list.empty()) {

                        data = std::move(data_bundle.data_list.front());
                        data_bundle.data_list.pop_front();
                    }

                    lck.unlock();
                }

                if (data) {

                    if (data_bundle.data_callback) {

                        data_bundle.data_callback(data);
                    }
                }
            }
        };

        if (!callback) {

            printf("Dataset: registerCallback, empty callback registered for %s\n", label.c_str());
            return false;
        }
        if (!this->data_pool_.contains(label)) {

            printf("Dataset: registerCallback, data %s is not supplied\n", label.c_str());
            return false;
        }
        auto &data = this->data_pool_[label];

        {
            std::lock_guard<std::mutex> lck(data.mt);
            if (data.post_process.joinable()) {
                
                data.exit = true;
                data.post_process.join();
                printf("Dataset registerCallback: stop previous post_process %s\n", label.c_str());
            }
        }
        {
            std::lock_guard<std::mutex> lck(data.mt);
            data.data_callback = callback;
            data.exit = false;
            data.post_process = std::thread {data_process, label};
            printf("Dataset registerCallback: start post_process %s\n", label.c_str());
        }

        return true;
    }

    auto Dataset::close() -> bool {

        for (auto &[label, data_bundle] : this->data_pool_) {

            {
                std::lock_guard<std::mutex> lck(data_bundle.mt);
                data_bundle.exit = true;
            }

            if (data_bundle.read_process.joinable()) {
                data_bundle.read_process.join();
            }
        }
        return true;
    }

    auto Dataset::startStreams() -> bool {

        if (!this->config_) {

            printf("Dataset startStreams, start with null config");
            return false;
        }

        for (auto &label : this->config_->enabled_labels) {

            if (!this->data_pool_.contains(label)) {

                printf("Dataset startStreams: stream %s is not suoported\n", label.c_str());
                return false;
            }

            auto &data_bundle = this->data_pool_[label];
            data_bundle.read_process = std::thread {data_bundle.job_read};
            printf("Dataset startStreams: stream %s started\n", label.c_str());
        }

        return true;
    }

    void Dataset::readImage(const std::string &image_label) {

        auto [img_data, img_timestamp] = this->config_->pathDataTimestamp(image_label);

        assert(std::filesystem::is_directory(img_data));
        assert(std::filesystem::is_regular_file(img_timestamp));

        auto timestamp_file = std::ifstream(img_timestamp);

        if (!timestamp_file) {

            printf("image [%s] timestamp_file can not be opened.\n", image_label.c_str());
            return;
        }

        std::string image_path;
        image_path.resize(img_data.size() + 16);
        unsigned idx = 0;
        for (std::string date_time_str; std::getline(timestamp_file, date_time_str);) {

            auto &image_data = this->data_pool_[image_label];

            if (image_data.exit) {

                printf("Dataset readImage: %s exit\n", image_label.c_str());
                break;
            }

            Timestamp timestamp(date_time_str);

            sprintf(image_path.data(), "%s/%010u%s", img_data.c_str(), idx++, IMG_EXT);
            std::ifstream img_file(image_path, std::ios::binary);

            if (!img_file.is_open()) {

                printf("Dataset: readImage, cannot read image: %s", image_path.c_str());
                break;
            }
            
            img_file.seekg(0, std::ios::end);
            u_int64_t len = img_file.tellg();
            img_file.seekg(0);

            std::vector<char> img_raw(len);
            img_file.read(img_raw.data(), len);
            
            auto image = std::make_unique<Image>(timestamp, img_raw);

            {
                std::lock_guard<std::mutex> lck(image_data.mt);
                image_data.data_list.emplace_back(std::move(image));
            }
            image_data.cv.notify_all();
            //printf("Dataset readImage: %s %s\n", image_label.c_str(), timestamp.printStr().c_str());
        }
    }

    void Dataset::readOxts() {

    }
}
