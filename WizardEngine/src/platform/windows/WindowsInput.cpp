//
// Created by mecha on 04.09.2021.
//

#include "WindowsInput.h"
#include "../../core/Application.h"

namespace engine {

    bool WindowsInput::isKeyPressed(KeyCode keyCode) {
        auto state = glfwGetKey(GLFW_WINDOW, keyCode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WindowsInput::isMousePressed(MouseCode mouseCode) {
        auto state = glfwGetMouseButton(GLFW_WINDOW, mouseCode);
        return state == GLFW_PRESS;
    }

    MousePosition WindowsInput::getMousePosition() {
        double x, y;
        glfwGetCursorPos(GLFW_WINDOW, &x, &y);
        return {
            (float) x,
            (float) y
        };
    }


}
