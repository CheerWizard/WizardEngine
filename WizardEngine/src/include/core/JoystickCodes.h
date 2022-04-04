//
// Created by mecha on 04.04.2022.
//

#pragma once

#include <core/primitives.h>

namespace engine::core {

    enum JoystickHat : u8 {
        HAT_CENTERED = 0,
        HAT_UP = 1,
        HAT_RIGHT = 2,
        HAT_DOWN = 4,
        HAT_LEFT = 8,

        HAT_RIGHT_UP = HAT_RIGHT | HAT_UP,
        HAT_RIGHT_DOWN = HAT_RIGHT | HAT_DOWN,
        HAT_LEFT_UP	= HAT_LEFT | HAT_UP,
        HAT_LEFT_DOWN = HAT_LEFT | HAT_DOWN
    };

}
