//
// Created by mecha on 04.09.2021.
//

#include "WindowsInput.h"

#include <GLFW/glfw3.h>

namespace engine {

    bool WindowsInput::isKeyPressed(KeyCode keyCode) {
        auto state = glfwGetKey((GLFWwindow*) nativeWindow, keyCode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WindowsInput::isMousePressed(MouseCode mouseCode) {
        auto state = glfwGetMouseButton((GLFWwindow*) nativeWindow, mouseCode);
        return state == GLFW_PRESS;
    }

    MousePosition WindowsInput::getMousePosition() {
        double x, y;
        glfwGetCursorPos((GLFWwindow*) nativeWindow, &x, &y);
        return {
            (float) x,
            (float) y
        };
    }


}
