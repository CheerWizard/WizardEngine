//
// Created by mecha on 29.08.2021.
//

#include "WindowsWindow.h"

#include "../../core/Memory.h"
#include "../../core/Assert.h"

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <stb_image.h>

#define GET_WINDOW_CALLBACK(x) (*(WindowProps*)glfwGetWindowUserPointer((GLFWwindow*) (x))).windowCallback
#define GET_KEYBOARD_CALLBACK(x) (*(WindowProps*)glfwGetWindowUserPointer((GLFWwindow*) (x))).keyboardCallback
#define GET_MOUSE_CALLBACK(x) (*(WindowProps*)glfwGetWindowUserPointer((GLFWwindow*) (x))).mouseCallback
#define GET_CURSOR_CALLBACK(x) (*(WindowProps*)glfwGetWindowUserPointer((GLFWwindow*) (x))).cursorCallback
#define NATIVE_WINDOW (GLFWwindow*)window

namespace engine {

    void WindowsWindow::create() {
        ENGINE_INFO("Creating window {0} ({1}, {2})", windowProps.title, windowProps.width, windowProps.height);

        if (!isInitialized) {
            ENGINE_ASSERT(glfwInit(), "Failed to initialize GLFW!");
            isInitialized = true;
            glfwSetErrorCallback(handleError);
        }

        setSampleSize(windowProps.sampleSize);

        window = glfwCreateWindow(
                (int)windowProps.width,
                (int)windowProps.height,
                windowProps.title.c_str(),
                nullptr,
                nullptr);
    }

    void WindowsWindow::enableFullScreen() {
        if (isFullScreen) return;
        isFullScreen = true;

        auto* monitor = glfwGetPrimaryMonitor();
        auto* videoMode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(NATIVE_WINDOW,
                             monitor,
                             0,
                             0,
                             videoMode->width,
                             videoMode->height,
                             videoMode->refreshRate);
        onWindowResized(videoMode->width, videoMode->height);
    }

    void WindowsWindow::disableFullScreen() {
        if (!isFullScreen) return;
        isFullScreen = false;

        int refreshRate = getRefreshRate();
        glfwSetWindowMonitor(NATIVE_WINDOW, nullptr, 0, 0, windowProps.width, windowProps.height, refreshRate);
        onWindowResized(windowProps.width, windowProps.height);
        setInCenter();
    }

    void WindowsWindow::destroy() {
        removeCallbacks();
        glfwSetErrorCallback(nullptr);
        glfwDestroyWindow(NATIVE_WINDOW);
        window = nullptr;
        glfwTerminate();
    }

    uint32_t WindowsWindow::getRefreshRate() {
        return glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate;
    }

    void WindowsWindow::onPrepare() {
        glfwSetWindowUserPointer(NATIVE_WINDOW, &windowProps);

        glfwSetWindowSizeCallback(NATIVE_WINDOW, [](GLFWwindow* window, int width, int height) {
            auto callback = GET_WINDOW_CALLBACK(window);

            if (callback != nullptr) {
                callback->onWindowResized(width, height);
            }
        });

        glfwSetWindowCloseCallback(NATIVE_WINDOW, [](GLFWwindow* window) {
            auto callback = GET_WINDOW_CALLBACK(window);

            if (callback != nullptr) {
                callback->onWindowClosed();
            }
        });

        glfwSetKeyCallback(NATIVE_WINDOW, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
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

        glfwSetCharCallback(NATIVE_WINDOW, [](GLFWwindow* window, unsigned int key) {
            auto callback = GET_KEYBOARD_CALLBACK(window);
            auto keycode = (KeyCode) key;

            if (callback != nullptr) {
                callback->onKeyTyped(keycode);
            }
        });

        glfwSetMouseButtonCallback(NATIVE_WINDOW, [](GLFWwindow* window, int button, int action, int mods) {
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

        glfwSetScrollCallback(NATIVE_WINDOW, [](GLFWwindow* window, double xOffset, double yOffset) {
            auto callback = GET_MOUSE_CALLBACK(window);

            if (callback != nullptr) {
                callback->onMouseScrolled(xOffset, yOffset);
            }
        });

        glfwSetCursorPosCallback(NATIVE_WINDOW, [](GLFWwindow* window, double xPos, double yPos) {
            auto callback = GET_CURSOR_CALLBACK(window);

            if (callback != nullptr) {
                callback->onCursorMoved(xPos, yPos);
            }
        });
    }

    void WindowsWindow::onUpdate() {
        glfwSwapBuffers(NATIVE_WINDOW);
        glfwPollEvents();
    }

    void WindowsWindow::onClose() {
        glfwSetWindowShouldClose(NATIVE_WINDOW, GLFW_TRUE);
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

    void WindowsWindow::setWindowIcon(const std::string &filePath) {
        GLFWimage images[1];
        images[0].pixels = stbi_load(filePath.c_str(),
                                     &images[0].width,
                                     &images[0].height,
                                     nullptr,
                                     4);
        glfwSetWindowIcon((GLFWwindow*) window, 1, images);
        stbi_image_free(images[0].pixels);
    }

    void WindowsWindow::onWindowResized(const uint32_t &width, const uint32_t &height) {
        auto callback = GET_WINDOW_CALLBACK(window);

        if (callback != nullptr) {
            callback->onWindowResized(width, height);
        }
    }

    void WindowsWindow::setSampleSize(const uint32_t &size) {
        glfwWindowHint(GLFW_SAMPLES, size);
    }

    void WindowsWindow::setPosition(const uint32_t &x, const uint32_t &y) {
        glfwSetWindowPos(NATIVE_WINDOW, x, y);
    }

}