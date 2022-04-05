//
// Created by mecha on 02.10.2021.
//

#pragma once

#include <core/primitives.h>

namespace engine::shader {

    using namespace core;

    struct UniformData {
        f32* uniforms;
        u32 index;
    };

}