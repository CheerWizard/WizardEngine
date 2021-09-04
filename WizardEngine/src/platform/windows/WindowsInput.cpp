//
// Created by mecha on 04.09.2021.
//

#include "WindowsInput.h"
#include "../../core/Application.h"

#include "GLFW/glfw3.h"

#define GLFW_WINDOW static_cast<GLFWwindow*>(Application::getInstance().getWindow().getNativeWindow())

namespace engine {

    Scope<Input> Input::newInstance() {
        return createScope<WindowsInput>();
    }

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
        return MousePosition((float) x, (float) y);
    }


}
