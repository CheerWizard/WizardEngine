//
// Created by mecha on 20.03.2022.
//

#include <core/Math.h>
#include "random"

namespace engine {

    void random(
            const int& minRange,
            const int& maxRange,
            const uint32_t& count,
            const std::function<void(const uint32_t& i, const float& random)>& callback
    ) {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(minRange, maxRange);
        for (uint32_t i = 0 ; i < count ; i++) {
            auto r = (float) dist(mt);
            callback(i, r);
        }
    }

    float random(const int& minRange, const int& maxRange) {
        return random((double)minRange, (double)maxRange);
    }

    float random(const double& minRange, const double& maxRange) {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(minRange, maxRange);
        return (float) dist(mt);
    }
}