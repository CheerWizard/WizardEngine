//
// Created by mecha on 29.08.2021.
//
#pragma once

#include <cstdint>

namespace engine {

    enum MouseCode : uint16_t {
        NoneButton             = 16,
        Button0                = 0,
        Button1                = 1,
        Button2                = 2,
        Button3                = 3,
        Button4                = 4,
        Button5                = 5,
        Button6                = 6,
        Button7                = 7,

        ButtonLast             = Button7,
        ButtonLeft             = Button0,
        ButtonRight            = Button1,
        ButtonMiddle           = Button2
    };

}