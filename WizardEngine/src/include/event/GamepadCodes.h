//
// Created by mecha on 04.04.2022.
//

#pragma once

#include <core/primitives.h>

namespace engine::event {

    enum ButtonState : core::u8 {
        PRESS = 1,
        RELEASE = 0,
        REPEAT = 2
    };

    enum GamepadButtonCode : core::u8 {
        PAD_BTN_A = 0,
        PAD_BTN_B = 1,
        PAD_BTN_X = 2,
        PAD_BTN_Y = 3,

        PAD_BTN_LEFT_BUMPER = 4,
        PAD_BTN_RIGHT_BUMPER = 5,

        PAD_BTN_BACK = 6,
        PAD_BTN_START = 7,
        PAD_BTN_GUIDE = 8,

        PAD_BTN_LEFT_THUMB = 9,
        PAD_BTN_RIGHT_THUMB = 10,

        PAD_BTN_D_PAD_UP = 11,
        PAD_BTN_D_PAD_RIGHT = 12,
        PAD_BTN_D_PAD_DOWN = 13,
        PAD_BTN_D_PAD_LEFT = 14,

        PAD_BTN_LAST = PAD_BTN_D_PAD_LEFT,

        PAD_BTN_CROSS = PAD_BTN_A,
        PAD_BTN_CIRCLE = PAD_BTN_B,
        PAD_BTN_SQUARE = PAD_BTN_X,
        PAD_BTN_TRIANGLE = PAD_BTN_Y
    };

    enum GamepadAxisCode : core::u8 {
        PAD_AXIS_LEFT_X = 0,
        PAD_AXIS_LEFT_Y = 1,
        PAD_AXIS_RIGHT_X = 2,
        PAD_AXIS_RIGHT_Y = 3,
        PAD_AXIS_LEFT_TRIGGER = 4,
        PAD_AXIS_RIGHT_TRIGGER = 5,
        PAD_AXIS_LAST = PAD_AXIS_RIGHT_TRIGGER
    };

}
