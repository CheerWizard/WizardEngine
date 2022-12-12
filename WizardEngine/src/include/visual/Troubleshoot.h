//
// Created by mecha on 03.09.2022.
//

#pragma once

#include <math/Math.h>

namespace engine::visual {

    using namespace math;

    class ENGINE_API ProfilerMenu final {

    public:
        static void draw(float&& width, float&& height);
    };

}