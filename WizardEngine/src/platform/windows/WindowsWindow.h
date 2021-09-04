//
// Created by mecha on 29.08.2021.
//
#pragma once

#include "../../core/Window.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace engine {

    class WindowsWindow : public Window {

    public:

        WindowsWindow(const WindowProps& windowProps) : Window(windowProps) {
            init(windowProps);
        }

        ~WindowsWindow() {
            shutdown();
        }

    public:
        void onUpdate() override;
        void enableVSync() override;
        void disableVSync() override;

    public:
        void* getNativeWindow() const override { return _window; }

    private:
        virtual void init(const WindowProps& windowProps);
        virtual void shutdown();

    private:
        static void handleError(int error, const char* description);

    private:
        GLFWwindow* _window;

    };

}