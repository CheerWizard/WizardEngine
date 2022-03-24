//
// Created by mecha on 20.03.2022.
//

#pragma once

#include <cstdint>
#include <functional>

namespace engine {
    void random(
            const int& minRange,
            const int& maxRange,
            const uint32_t& count,
            const std::function<void(const uint32_t& i, const float& random)>& callback
    );
    float random(const double& minRange, const double& maxRange);
    float random(const int& minRange, const int& maxRange);
}