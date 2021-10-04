//
// Created by mecha on 29.08.2021.
//
#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"

namespace engine {

    class WindowCallback {
    public:
        virtual void onWindowClosed() = 0;
        virtual void onWindowResized(unsigned int width, unsigned int height) = 0;
    };

    class KeyboardCallback {
    public:
        virtual void onKeyPressed(KeyCode keyCode) = 0;
        virtual void onKeyHold(KeyCode keyCode) = 0;
        virtual void onKeyReleased(KeyCode keyCode) = 0;
        virtual void onKeyTyped(KeyCode keyCode) = 0;
    };

    class MouseCallback {
    public:
        virtual void onMousePressed(MouseCode mouseCode) = 0;
        virtual void onMouseRelease(MouseCode mouseCode) = 0;
        virtual void onMouseScrolled(double xOffset, double yOffset) = 0;
    };

    class CursorCallback {
    public:
        virtual void onCursorMoved(double xPos, double yPos) = 0;
    };

}