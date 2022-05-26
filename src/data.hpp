/**
 * Author: Cyan
 * Date: Wed May 25 07:14:25 AM UTC 2022
 */
#pragma once
#include <memory>

namespace kitti_raw {

    class Data {

    public:
        virtual ~Data() = default;

        template <typename T>
        auto cast() -> T& {

            return *std::dynamic_pointer_cast<T>(this);
        };
    };
}
