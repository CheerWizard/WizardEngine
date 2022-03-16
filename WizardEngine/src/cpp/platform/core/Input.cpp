//
// Created by mecha on 04.09.2021.
//

#include <platform/core/Input.h>
#include <GLFW/glfw3.h>

namespace engine {

    bool Input::isKeyPressed(KeyCode keyCode) {
        auto state = glfwGetKey((GLFWwindow*) nativeWindow, keyCode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::isMousePressed(MouseCode mouseCode) {
        auto state = glfwGetMouseButton((GLFWwindow*) nativeWindow, mouseCode);
        return state == GLFW_PRESS;
    }

    MousePosition Input::getMousePosition() {
        double x, y;
        glfwGetCursorPos((GLFWwindow*) nativeWindow, &x, &y);
        return {
            (float) x,
            (float) y
        };
    }

    void Input::destroy() {
        nativeWindow = nullptr;
    }
}