//
// Created by mecha on 07.01.2022.
//

#include "Window.h"

namespace engine {

    void Window::removeCallbacks() {
        removeWindowCallback();
        removeKeyboardCallback();
        removeMouseCallback();
        removeCursorCallback();
    }

    void Window::toggleFullScreen() {
        isFullScreen = !isFullScreen;
        if (isFullScreen) {
            enableFullScreen();
        } else {
            disableFullScreen();
        }
    }

    void Window::setInCenter() {
        setPosition(windowProps.width / 2, windowProps.height / 2);
    }

}