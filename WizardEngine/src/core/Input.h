//
// Created by mecha on 04.09.2021.
//

#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"

namespace engine {

    struct MousePosition {
        float x;
        float y;
    };

    class Input {

    public:
        Input(void* nativeWindow) :
        nativeWindow(nativeWindow) {}

        virtual ~Input() {
            destroy();
        }

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

    private:
        void destroy();

    protected:
        void* nativeWindow;

    };

}
