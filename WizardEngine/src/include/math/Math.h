//
// Created by mecha on 20.03.2022.
//

#pragma once

#include <core/primitives.h>
#include <functional>
#include <glm/glm.hpp>

namespace engine::math {
    using namespace core;

    void random(
            const s32& minRange,
            const s32& maxRange,
            const u32& count,
            const std::function<void(const u32& i, const f32& random)>& callback
    );
    f32 random(const double& minRange, const double& maxRange);
    f32 random(const s32& minRange, const s32& maxRange);
}