/**
 * Author: Cyan
 * Date: Thu May 26 08:56:23 AM UTC 2022
 */
#include <assert.h>
#include <exception>
#include <filesystem>

#include "config.hpp"
#include "define.hpp"


int main(int argc, char *argv[]) {

    if (argc == 1) {

        std::unique_ptr<kitti_raw::Config> config = kitti_raw::Config::Default();

        auto labels = {
            kitti_raw::IMAGE_00,
            kitti_raw::OXTS,
        };
        try {

            for (auto &label : labels) {

                auto [data_path, timestamps] = config->pathDataTimestamp(label);
                for (auto &file : std::filesystem::directory_iterator(data_path)) {

                    assert(file.exists());
                    assert(file.is_regular_file());
                    assert(file.file_size() != 0);
                }
                assert(std::filesystem::is_regular_file(timestamps));
                assert(std::filesystem::file_size(timestamps) != 0);
            }
        } catch (const std::exception &e) {

            std::string err_str = e.what();
            assert(false);
        }
    } else {

    }

    return 0;
}
