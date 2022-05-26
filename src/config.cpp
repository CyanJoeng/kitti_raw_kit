/**
 * Author: Cyan
 * Date: Tue May 24 03:51:05 AM UTC 2022
 */
#include "config.hpp"
#include "define.hpp"

#include <memory>
#include <sstream>
#include <stdexcept>
#include <filesystem>

namespace kitti_raw {

    auto Config::LoadFromFile(const std::string &config_file) -> std::unique_ptr<Config> {

        auto config = std::make_unique<Config>();

        return config;
    }

    auto Config::Default() -> std::unique_ptr<Config> {

        auto config = std::make_unique<Config>();

        config->base_dir = "/data";
        config->category = "city";
        config->date = "2011_09_26";
        config->enabled_labels.emplace(IMAGE_00);
        config->enabled_labels.emplace(OXTS);

        return config;
    }

    auto Config::IdDir(const std::string &date, const int &id) -> std::string {

        std::array<char, 64> str;
        sprintf(str.data(), "%s_drive_%04u", date.data(), id);
        return str.data();
    }

    auto Config::SyncOrExtractDir(const std::string &date, const int &id, const bool &is_sync) -> std::string {

        auto id_dir = Config::IdDir(date, id);
        auto sync_str = is_sync? SYNC: EXTRACT;

        std::array<char, 64> str;
        sprintf(str.data(), "%s_%s", id_dir.data(), sync_str);

        return str.data();
    }

    auto Config::pathDataTimestamp(const std::string &label) const -> std::pair<std::string, std::string> {

        if (! this->enabled_labels.count(label)) {

            std::stringstream ss;
            ss << label << " not enabled.";
            throw std::out_of_range(ss.str());
        }

        std::filesystem::path path(this->base_dir);

        auto id_dir = Config::IdDir(this->date, this->serial_id);
        auto sync_dir = Config::SyncOrExtractDir(this->date, this->serial_id, this->is_sync);
        auto dir = path / this->category / id_dir / this->date / sync_dir / label;

        if (!std::filesystem::is_directory(dir)) {

            std::stringstream ss;
            ss << dir << ", is not a trully dir.";
            throw std::runtime_error(ss.str());
        }

        return {dir / DATA_DIR, dir / TIMESTAMPS_FILE};
    }
}
