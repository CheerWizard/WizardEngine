//
// Created by mecha on 04.09.2021.
//

#pragma once

#include <core/KeyCodes.h>
#include <core/MouseCodes.h>

namespace engine {

    struct MousePosition {
        float x;
        float y;
    };

    class Input {

    public:
        Input(void* nativeWindow) :
        nativeWindow(nativeWindow) {}

        ~Input() {
            destroy();
        }

    public:
        bool isKeyPressed(KeyCode keyCode);
        bool isMousePressed(MouseCode mouseCode);
        MousePosition getMousePosition();

    public:
        inline float getMousePosX() {
            return getMousePosition().x;
        }

        inline float getMousePosY() {
            return getMousePosition().y;
        }

    private:
        void destroy();

    protected:
        void* nativeWindow;

    };

}
