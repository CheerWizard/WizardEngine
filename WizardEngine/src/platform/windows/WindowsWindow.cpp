//
// Created by mecha on 29.08.2021.
//

#include "WindowsWindow.h"

#include "../../core/Logger.h"
#include "../../core/Assert.h"

#include "../../events/KeyEvent.h"
#include "../../events/MouseEvent.h"

#include <glad/glad.h>

#define GET_WINDOW_CALLBACK(...) (*(WindowProps*)glfwGetWindowUserPointer(__VA_ARGS__)).windowCallback
#define GET_EVENT_CALLBACK(...) (*(WindowProps*)glfwGetWindowUserPointer(__VA_ARGS__)).eventCallback

namespace engine {

    Scope<Window> Window::newInstance(const WindowProps& props) {
        return createScope<WindowsWindow>(props);
    }

    void WindowsWindow::onCreate() {
        ENGINE_INFO("Creating window {0} ({1}, {2})", windowProps.title, windowProps.width, windowProps.height);

        if (!isInitialized) {
            int glfwCreated = glfwInit();
            ENGINE_ASSERT(glfwCreated, "Failed to initialize GLFW!")
            isInitialized = true;
            glfwSetErrorCallback(handleError);
        }

        _window = glfwCreateWindow(
                (int)windowProps.width,
                (int)windowProps.height,
                windowProps.title.c_str(),
                nullptr,
                nullptr);
        glfwMakeContextCurrent(_window);

        int gladCreated = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        ENGINE_ASSERT(gladCreated, "Failed to initialize Glad!");

        glfwSetWindowUserPointer(_window, &windowProps);
        enableVSync();

        glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
            auto callback = GET_WINDOW_CALLBACK(window);
            if (callback != nullptr) {
                callback->onWindowResized(width, height);
            }
            //            (*(WindowProps*)glfwGetWindowUserPointer(window)).windowCallback->onWindowResized(width, height);
        });

        glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
            auto callback = GET_WINDOW_CALLBACK(window);
            if (callback != nullptr) {
                callback->onWindowClosed();
            }
            //            (*(WindowProps*)glfwGetWindowUserPointer(window)).windowCallback->onWindowClosed();
        });

        glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key, 0);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent event(key, 1);
                    break;
                }
            }
        });

        glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int keycode) {
            KeyTypedEvent event(keycode);
        });

        glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(button);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(button);
                    break;
                }
            }
        });

        glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset) {
            MouseScrollEvent event((float)xOffset, (float)yOffset);
        });

        glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos) {
            MouseMoveEvent event((float)xPos, (float)yPos);
        });
    }

    void WindowsWindow::onUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(_window);
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void WindowsWindow::onDestroy() {
        Window::onDestroy();
        glfwDestroyWindow(_window);
        glfwTerminate();
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