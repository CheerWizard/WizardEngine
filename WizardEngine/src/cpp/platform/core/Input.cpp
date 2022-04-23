//
// Created by mecha on 04.09.2021.
//

#include <platform/core/Input.h>
#include <GLFW/glfw3.h>

namespace engine::event {

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
        return { (f32) x, (f32) y };
    }

    void Input::destroy() {
        nativeWindow = nullptr;
    }

    bool Input::isJoystickConnected() const {
        return glfwJoystickPresent(joystickId) == 1;
    }

    JoystickAxisStates Input::getJoystickAxisStates() const {
        s32 count;
        const f32* axes = glfwGetJoystickAxes(joystickId, &count);
        return { axes, count };
    }

    JoystickButtonStates Input::getJoystickButtonStates() const {
        s32 count;
        const u8* buttons = glfwGetJoystickButtons(joystickId, &count);\
        return { buttons, count };
    }

    JoystickHatStates Input::getJoystickHatStates() const {
        s32 count;
        const u8* hats = glfwGetJoystickHats(joystickId, &count);
        return { hats, count };
    }

    const char* Input::getJoystickName() const {
        return glfwGetJoystickName(joystickId);
    }

    bool Input::isGamepadPresent() const {
        return glfwJoystickIsGamepad(joystickId);
    }

    const char* Input::getGamepadName() const {
        return glfwGetGamepadName(joystickId);
    }

    GamepadState Input::getGamepadState() const {
        GLFWgamepadstate state;
        glfwGetGamepadState(joystickId, &state);
        GamepadState gamepadState {};

        for (u32 i = 0 ; i < 6 ; i++) {
            gamepadState.buttons[i] = state.buttons[i];
            gamepadState.axes[i] = state.axes[i];
        }
        for (u32 i = 6 ; i < 15 ; i++) {
            gamepadState.buttons[i] = state.buttons[i];
        }

        return gamepadState;
    }

    bool Input::isJoystickDisconnected() const {
        return glfwJoystickPresent(joystickId) != 1;
    }
}