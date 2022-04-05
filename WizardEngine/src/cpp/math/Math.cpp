//
// Created by mecha on 20.03.2022.
//

#include <math/Math.h>
#include <random>

namespace engine::math {

    void random(
            const s32& minRange,
            const s32& maxRange,
            const u32& count,
            const std::function<void(const u32& i, const f32& random)>& callback
    ) {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(minRange, maxRange);
        for (u32 i = 0 ; i < count ; i++) {
            auto r = (float) dist(mt);
            callback(i, r);
        }
    }

    f32 random(const s32& minRange, const s32& maxRange) {
        return random((double)minRange, (double)maxRange);
    }

    f32 random(const double& minRange, const double& maxRange) {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<double> dist(minRange, maxRange);
        return (f32) dist(mt);
    }
}