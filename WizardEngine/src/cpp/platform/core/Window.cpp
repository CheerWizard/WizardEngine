//
// Created by mecha on 29.08.2021.
//

#include <platform/core/Window.h>
#include <core/Assert.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <core/Application.h>
#include <core/filesystem.h>

#define NATIVE_WINDOW (GLFWwindow*)window

namespace engine::core {

    Window::Window(const WindowProps &windowProps) {
        ENGINE_INFO("Creating window {0} ({1}, {2})", windowProps.title.c_str(), windowProps.width, windowProps.height);
        this->windowProps = windowProps;

        if (!isInitialized) {
            isInitialized = glfwInit() != 0;
            ENGINE_ASSERT(isInitialized, "Failed to initialize GLFW!");
            glfwSetErrorCallback(handleError);
        }

        setSampleSize(windowProps.sampleSize);

        window = glfwCreateWindow(
                windowProps.width,
                windowProps.height,
                windowProps.title.c_str(),
                nullptr,
                nullptr);

        windowProps.fullscreen ? enableFullScreen() : disableFullScreen();

// ---------- hide main window cursor, use ImGui cursor instead -------------
#ifdef VISUAL
        glfwSetInputMode(NATIVE_WINDOW, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
#endif
    }

    Window::~Window() {
        glfwSetErrorCallback(nullptr);
        glfwDestroyWindow(NATIVE_WINDOW);
        glfwTerminate();
    }

    void Window::enableFullScreen() {
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

    void Window::disableFullScreen() {
        if (!isFullScreen) return;
        isFullScreen = false;

        int refreshRate = getRefreshRate();
        glfwSetWindowMonitor(NATIVE_WINDOW, nullptr, 0, 0, windowProps.width, windowProps.height, refreshRate);
        onWindowResized(windowProps.width, windowProps.height);
        setInCenter();
    }

    int Window::getRefreshRate() {
        return glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate;
    }

    void Window::onPrepare() {
        glfwSetWindowUserPointer(NATIVE_WINDOW, &windowProps);

        glfwSetWindowSizeCallback(NATIVE_WINDOW, [](GLFWwindow* window, int width, int height) {
            Application::get().onWindowResized(width, height);
        });

        glfwSetWindowCloseCallback(NATIVE_WINDOW, [](GLFWwindow* window) {
            Application::get().onWindowClosed();
        });

        glfwSetKeyCallback(NATIVE_WINDOW, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            auto keycode = (event::KeyCode) key;
            switch (action) {
                case GLFW_PRESS: {
                    Application::get().onKeyPressed(keycode);
                    break;
                }
                case GLFW_RELEASE: {
                    Application::get().onKeyReleased(keycode);
                    break;
                }
                case GLFW_REPEAT: {
                    Application::get().onKeyHold(keycode);
                    break;
                }
                default: break;
            }
        });

        glfwSetCharCallback(NATIVE_WINDOW, [](GLFWwindow* window, unsigned int key) {
            Application::get().onKeyTyped((event::KeyCode) key);
        });

        glfwSetMouseButtonCallback(NATIVE_WINDOW, [](GLFWwindow* window, int button, int action, int mods) {
            auto mouseCode = (event::MouseCode) button;
            switch (action) {
                case GLFW_PRESS: {
                    Application::get().onMousePressed(mouseCode);
                    break;
                }
                case GLFW_RELEASE: {
                    Application::get().onMouseRelease(mouseCode);
                    break;
                }
                default: break;
            }
        });

        glfwSetScrollCallback(NATIVE_WINDOW, [](GLFWwindow* window, double xOffset, double yOffset) {
            Application::get().onMouseScrolled(xOffset, yOffset);
        });

        glfwSetCursorPosCallback(NATIVE_WINDOW, [](GLFWwindow* window, double xPos, double yPos) {
            Application::get().onCursorMoved(xPos, yPos);
        });

        glfwSetJoystickCallback([](int jid, int event) {
            if (event == GLFW_CONNECTED) {
                Application::get().onGamepadConnected(jid);
                return;
            }

            if (event == GLFW_DISCONNECTED) {
                Application::get().onGamepadDisconnected(jid);
                return;
            }
        });
    }

    void Window::onUpdate() {
        glfwSwapBuffers(NATIVE_WINDOW);
        glfwPollEvents();
    }

    void Window::onClose() {
        glfwSetWindowShouldClose(NATIVE_WINDOW, GLFW_TRUE);
    }

    void Window::enableVSync() {
        windowProps.vSyncEnabled = true;
        glfwSwapInterval(1);
    }

    void Window::disableVSync() {
        windowProps.vSyncEnabled = false;
        glfwSwapInterval(0);
    }

    void Window::handleError(int error, const char *description) {
        ENGINE_ERR("GLFW Error ({0}): {1}", error, description);
    }

    void Window::setWindowIcon(const std::string &filePath) {
        GLFWimage images[1];
        images[0].pixels = stbi_load(filePath.c_str(),
                                     &images[0].width,
                                     &images[0].height,
                                     nullptr,
                                     4);
        glfwSetWindowIcon((GLFWwindow*) window, 1, images);
        stbi_image_free(images[0].pixels);
    }

    void Window::setSampleSize(int size) {
        windowProps.sampleSize = size;
        glfwWindowHint(GLFW_SAMPLES, size);
    }

    void Window::setPosition(int x, int y) {
        xPos = x;
        yPos = y;
        glfwSetWindowPos(NATIVE_WINDOW, x, y);
    }

    void Window::getPosition(int& x, int& y) {
        x = xPos;
        y = yPos;
    }

    void Window::onWindowResized(int width, int height) {
        Application::get().onWindowResized(width, height);
    }

    void Window::toggleFullScreen() {
        isFullScreen = !isFullScreen;
        if (isFullScreen) {
            enableFullScreen();
        } else {
            disableFullScreen();
        }
    }

    void Window::setInCenter() {
        setPosition(windowProps.width / 2, windowProps.height / 2);
    }

    void Window::loadGamepadMappings(const char* filePath) {
        auto mappings = engine::filesystem::read(filePath);
        glfwUpdateGamepadMappings(mappings.c_str());
    }

}