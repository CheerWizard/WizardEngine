//
// Created by mecha on 04.09.2021.
//

#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"
#include "Memory.h"
#include "Logger.h"
#include "../platform/Platform.h"

#include <string>

namespace engine {

    struct MousePosition {
        float x;
        float y;

        MousePosition(float x, float y) : x(x), y(y) {}

        void log() {
            ENGINE_INFO("Mouse [x : {0}, y : {1}]", x, y);
        }
    };

    class Input {

    public:
        Input() = default;;
        virtual ~Input() = default;

    public:
        virtual bool isKeyPressed(KeyCode keyCode) = 0;
        virtual bool isMousePressed(MouseCode mouseCode) = 0;
        virtual MousePosition getMousePosition() = 0;

    public:
        inline float getMousePosX() {
            return getMousePosition().x;
        }

        inline float getMousePosY() {
            return getMousePosition().y;
        }
    };

}
