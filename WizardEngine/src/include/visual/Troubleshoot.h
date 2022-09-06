//
// Created by mecha on 03.09.2022.
//

#pragma once

#include <math/Math.h>

namespace engine::visual {

    using namespace math;

    class ProfilerMenu final {

    public:
        static void draw(const char* title, const vec2f& size);
    };

}