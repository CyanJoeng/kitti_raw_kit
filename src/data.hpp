/**
 * Author: Cyan
 * Date: Wed May 25 07:14:25 AM UTC 2022
 */
#pragma once
#include <memory>

#include "timestamp.hpp"

namespace kitti_raw {

    class Data {

    public:
        explicit Data(const Timestamp &);

        virtual ~Data() = default;

        template <typename T>
        auto cast() -> T& {

            return dynamic_cast<T&>(*this);
        };

        virtual auto isValid() const -> bool = 0;

        auto timestamp() const -> const Timestamp&;

    private:
        Timestamp timestamp_;
    };
}
