//
// Created by mecha on 29.08.2021.
//

#include "WindowsWindow.h"

#include "../../core/Memory.h"
#include "../../core/Assert.h"

#include <GLFW/glfw3.h>

#define GET_WINDOW_CALLBACK(...) (*(WindowProps*)glfwGetWindowUserPointer(__VA_ARGS__)).windowCallback
#define GET_KEYBOARD_CALLBACK(...) (*(WindowProps*)glfwGetWindowUserPointer(__VA_ARGS__)).keyboardCallback
#define GET_MOUSE_CALLBACK(...) (*(WindowProps*)glfwGetWindowUserPointer(__VA_ARGS__)).mouseCallback
#define GET_CURSOR_CALLBACK(...) (*(WindowProps*)glfwGetWindowUserPointer(__VA_ARGS__)).cursorCallback

namespace engine {

    void WindowsWindow::create() {
        ENGINE_INFO("Creating window {0} ({1}, {2})", windowProps.title, windowProps.width, windowProps.height);

        if (!isInitialized) {
            ENGINE_ASSERT(glfwInit(), "Failed to initialize GLFW!")
            isInitialized = true;
            glfwSetErrorCallback(handleError);
        }

        _window = glfwCreateWindow(
                (int)windowProps.width,
                (int)windowProps.height,
                windowProps.title.c_str(),
                nullptr,
                nullptr);
    }

    void WindowsWindow::destroy() {
        removeCallbacks();
        glfwDestroyWindow(_window);
        _window = nullptr;
        glfwTerminate();
    }

    uint32_t WindowsWindow::getRefreshRate() {
        return glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate;
    }

    void WindowsWindow::onPrepare() {
        glfwSetWindowUserPointer(_window, &windowProps);

        glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
            auto callback = GET_WINDOW_CALLBACK(window);

            if (callback != nullptr) {
                callback->onWindowResized(width, height);
            }
        });

        glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
            auto callback = GET_WINDOW_CALLBACK(window);

            if (callback != nullptr) {
                callback->onWindowClosed();
            }
        });

        glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            auto callback = GET_KEYBOARD_CALLBACK(window);

            if (callback != nullptr) {
                auto keycode = (KeyCode) key;

                switch (action) {
                    case GLFW_PRESS: {
                        callback->onKeyPressed(keycode);
                        break;
                    }
                    case GLFW_RELEASE: {
                        callback->onKeyReleased(keycode);
                        break;
                    }
                    case GLFW_REPEAT: {
                        callback->onKeyHold(keycode);
                        break;
                    }
                }
            }
        });

        glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int key) {
            auto callback = GET_KEYBOARD_CALLBACK(window);
            auto keycode = (KeyCode) key;

            if (callback != nullptr) {
                callback->onKeyTyped(keycode);
            }
        });

        glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
            auto callback = GET_MOUSE_CALLBACK(window);

            if (callback != nullptr) {
                auto mouseCode = (MouseCode) button;

                switch (action) {
                    case GLFW_PRESS: {
                        callback->onMousePressed(mouseCode);
                        break;
                    }
                    case GLFW_RELEASE: {
                        callback->onMouseRelease(mouseCode);
                        break;
                    }
                }
            }
        });

        glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset) {
            auto callback = GET_MOUSE_CALLBACK(window);

            if (callback != nullptr) {
                callback->onMouseScrolled(xOffset, yOffset);
            }
        });

        glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos) {
            auto callback = GET_CURSOR_CALLBACK(window);

            if (callback != nullptr) {
                callback->onCursorMoved(xPos, yPos);
            }
        });
    }

    void WindowsWindow::onUpdate() {
        glfwPollEvents();
    }

    void WindowsWindow::onClose() {
        glfwSetWindowShouldClose(_window, GLFW_TRUE);
    }

    void WindowsWindow::enableVSync() {
        Window::enableVSync();
        glfwSwapInterval(1);
    }

    void WindowsWindow::disableVSync() {
        Window::disableVSync();
        glfwSwapInterval(0);
    }

    void WindowsWindow::handleError(int error, const char *description) {
        ENGINE_ERR("GLFW Error ({0}): {1}", error, description);
    }

}