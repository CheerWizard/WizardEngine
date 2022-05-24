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

        PAD_BTN_UP = 11,
        PAD_BTN_RIGHT = 12,
        PAD_BTN_DOWN = 13,
        PAD_BTN_LEFT = 14,

        PAD_BTN_LAST = PAD_BTN_LEFT,

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

    struct GamepadRollCode {
        core::u8 x, y, trigger;

        bool operator==(const GamepadRollCode& other) const {
            return x == other.x && y == other.y && trigger == other.trigger;
        }
    };

    constexpr GamepadRollCode PAD_LEFT_ROLL = { PAD_AXIS_LEFT_X, PAD_AXIS_LEFT_Y, PAD_AXIS_LEFT_TRIGGER };
    constexpr GamepadRollCode PAD_RIGHT_ROLL = { PAD_AXIS_RIGHT_X, PAD_AXIS_RIGHT_Y, PAD_AXIS_RIGHT_TRIGGER };

    struct GamepadRoll {
        core::f32 x, y;
        bool triggered;

        bool operator==(const GamepadRoll& other) const {
            return x == other.x && y == other.y && triggered == other.triggered;
        }

        bool operator!=(const GamepadRoll& other) const {
            return !operator==(other);
        }
    };
}

namespace std {
    template<>
    struct hash<engine::event::GamepadRollCode> {
        size_t operator()(const engine::event::GamepadRollCode& key) const {
            return key.x + key.y + key.trigger;
        }
    };
}