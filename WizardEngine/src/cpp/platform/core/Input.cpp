//
// Created by mecha on 04.09.2021.
//

#include <platform/core/Input.h>
#include <GLFW/glfw3.h>

namespace engine::event {

    MousePosition MousePosition::operator-(const MousePosition &mousePosition) const {
        return { x - mousePosition.x, y - mousePosition.y };
    }

    void* Input::nativeWindow = nullptr;
    s32 Input::joystickId = 0;
    MousePosition Input::previousMousePosition;

    void Input::create(void *nativeWindow) {
        Input::nativeWindow = nativeWindow;
    }

    void Input::destroy() {
        nativeWindow = nullptr;
    }

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

    MousePosition Input::getMouseDelta() {
        auto mousePosition = getMousePosition();
        auto delta = (mousePosition - previousMousePosition) * 0.003f;
        previousMousePosition = mousePosition;
        return delta;
    }

    bool Input::isJoystickConnected() {
        return glfwJoystickPresent(joystickId) == 1;
    }

    JoystickAxisStates Input::getJoystickAxisStates() {
        s32 count;
        const f32* axes = glfwGetJoystickAxes(joystickId, &count);
        return { axes, count };
    }

    JoystickButtonStates Input::getJoystickButtonStates() {
        s32 count;
        const u8* buttons = glfwGetJoystickButtons(joystickId, &count);
        return { buttons, count };
    }

    JoystickHatStates Input::getJoystickHatStates() {
        s32 count;
        const u8* hats = glfwGetJoystickHats(joystickId, &count);
        return { hats, count };
    }

    const char* Input::getJoystickName() {
        return glfwGetJoystickName(joystickId);
    }

    bool Input::isGamepadPresent() {
        return glfwJoystickIsGamepad(joystickId);
    }

    const char* Input::getGamepadName() {
        return glfwGetGamepadName(joystickId);
    }

    GamepadState Input::getGamepadState() {
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

    bool Input::isJoystickDisconnected() {
        return glfwJoystickPresent(joystickId) != 1;
    }
}