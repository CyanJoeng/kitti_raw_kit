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
#include <functional>
#include <condition_variable>

#include "config.hpp"
#include "data.hpp"

namespace kitti_raw {

    class Dataset {

    public:
        using Callback = std::function<void (const std::shared_ptr<Data> &data)>;

    private:
        Dataset();

        Dataset(const Dataset&) = delete;
        Dataset& operator=(const Dataset&) = delete;

    public:
        static auto Inst() -> Dataset&;

        ~Dataset();

        /**
         * open dataset streams
         * return false if any stream can not be opened
         */
        auto open(const std::string &config_file = "") -> bool;

        auto registerCallback(const std::string &label, const Callback &callback) -> bool;

        auto close() -> bool;

    private:
        auto startStreams() -> bool;

        void readImage(const std::string &image_label);

        void readOxts();

    private:
        std::unique_ptr<Config> config_;

        struct _DataBundle {

            std::function<void ()> job_read;
            Callback data_callback;

            std::thread read_process;
            std::list<std::unique_ptr<Data>> data_list;

            std::thread post_process;
            bool exit {false};

            std::mutex mt;
            std::condition_variable cv;
        };
        std::map<std::string, _DataBundle> data_pool_;
    };
}
