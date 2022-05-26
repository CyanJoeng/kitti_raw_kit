/**
 * Author: Cyan
 * Date: Tue May 24 03:35:30 AM UTC 2022
 */
#pragma once
#include <map>
#include <list>
#include <mutex>
#include <string>
#include <memory>
#include <thread>
#include <condition_variable>

#include "config.hpp"
#include "data.hpp"

namespace kitti_raw {

    class Dataset {

    public:
        static auto Inst() -> Dataset&;

        /**
         * open dataset streams
         * return false if any stream can not be opened
         */
        auto open(const std::string &config_file = "") -> bool;

    private:
        void readImage(const std::string &image_label);

        void readImu();

    private:
        std::unique_ptr<Config> config_;

        struct _Data {

            std::thread th;
            std::list<std::unique_ptr<Data>> data_list;

            std::mutex mt;
            std::condition_variable cv;
        };
        std::map<std::string, _Data> thread_pool_;
    };
}
