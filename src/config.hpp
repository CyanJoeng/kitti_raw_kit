/**
 * Author: Cyan
 * Date: Tue May 24 03:44:04 AM UTC 2022
 */
#pragma once
#include <set>
#include <string>
#include <memory>

#include "define.hpp"

namespace kitti_raw {

    struct Config {

        static auto LoadFromFile(const std::string &config_file) -> std::unique_ptr<Config>;
        
        static auto Default() -> std::unique_ptr<Config>;

        static auto IdDir(const std::string &date, const int &id) -> std::string;

        static auto SyncOrExtractDir(const std::string &date, const int &id, const bool &is_sync) -> std::string;

        auto pathDataTimestamp(const std::string &label) const -> std::pair<std::string, std::string>;


        std::string base_dir;

        std::string category;

        std::string date;

        int serial_id {1};

        bool is_sync {true};

        std::set<std::string> enabled_labels;
    };
}
