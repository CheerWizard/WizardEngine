//
// Created by mecha on 29.08.2021.
//

#include "WindowsWindow.h"

#include "../../core/Logger.h"
#include "../../core/Assert.h"

#include "../../events/KeyEvent.h"
#include "../../events/MouseEvent.h"
#include "../../events/WindowEvent.h"

#include <glad/glad.h>

namespace engine {

    Scope<Window> Window::newInstance(const WindowProps& props) {
        return createScope<WindowsWindow>(props);
    }

    void WindowsWindow::init(const WindowProps &props) {
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
        ENGINE_ASSERT(gladCreated, "Failed to initialize Glad!")

        glfwSetWindowUserPointer(_window, &windowProps);
        enableVSync();

        glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
            WindowResizeEvent event(width, height);
//            onEvent(event);
        });

        glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
            WindowCloseEvent event;
//            onEvent(event);
        });

        glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key, 0);
//                    onEvent(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
//                    onEvent(event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent event(key, 1);
//                    onEvent(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int keycode) {
            KeyTypedEvent event(keycode);
//            onEvent(event);
        });

        glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(button);
//                    onEvent(event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(button);
//                    onEvent(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset) {
            MouseScrollEvent event((float)xOffset, (float)yOffset);
//            onEvent(event);
        });

        glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos) {
            MouseMoveEvent event((float)xPos, (float)yPos);
//            onEvent(event);
        });
    }

    void WindowsWindow::handleError(int error, const char *description) {
        ENGINE_ERR("GLFW Error ({0}): {1}", error, description);
    }

    void WindowsWindow::shutdown() {
        glfwDestroyWindow(_window);
        glfwTerminate();
    }

    void WindowsWindow::onUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(_window);
        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void WindowsWindow::enableVSync() {
        Window::enableVSync();
        glfwSwapInterval(1);
    }

    void WindowsWindow::disableVSync() {
        Window::disableVSync();
        glfwSwapInterval(0);
    }

}