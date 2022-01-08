//
// Created by mecha on 02.10.2021.
//

#pragma once

namespace engine::shader {

    enum ElementCount : uint8_t {
        NONE = 0,
        SINGLE = 1,
        DOUBLE = 2,
        VEC2 = 2,
        D_VEC2 = 4,
        VEC3 = 3,
        D_VEC3 = 6, // double = 2 floats!
        VEC4 = 4,
        D_VEC4 = 8, // double = 2 floats!
        MAT2 = 4,
        MAT3 = 9,
        MAT4 = 16
    };

}