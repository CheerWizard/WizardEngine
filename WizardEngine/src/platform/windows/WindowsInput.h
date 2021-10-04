//
// Created by mecha on 04.09.2021.
//

#pragma once

#include "../../core/Input.h"

namespace engine {

    class WindowsInput : public Input {

    public:
        WindowsInput(void* nativeWindow) : Input(nativeWindow) {}

        ~WindowsInput() override = default;

    public:
        bool isKeyPressed(KeyCode keyCode) override;
        bool isMousePressed(MouseCode mouseCode) override;
        MousePosition getMousePosition() override;

    };

}