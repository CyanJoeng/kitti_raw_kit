/**
 * Author: Cyan
 * Date: Tue May 24 03:38:34 AM UTC 2022
 */
#include "dataset.hpp"

#include <assert.h>

#include <memory>
#include <fstream>
#include <exception>
#include <filesystem>

#include "config.hpp"


namespace kitti_raw {

    auto Dataset::Inst() -> Dataset& {

        static Dataset inst;
        return inst;
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

        return true;
    }

    void Dataset::readImage(const std::string &image_label) {

        auto [img_data, img_timestamp] = this->config_->pathDataTimestamp(image_label);

        assert(std::filesystem::is_directory(img_data));
        assert(std::filesystem::is_regular_file(img_timestamp));

        unsigned idx = 0;

        auto timestamp_file = std::ifstream(img_timestamp);

        if (!timestamp_file) {

            printf("image [%s] timestamp_file can not be opened.\n", image_label.c_str());
            return;
        }

        for (std::string date, time; timestamp_file >> date >> time;) {


        }
    }
}
