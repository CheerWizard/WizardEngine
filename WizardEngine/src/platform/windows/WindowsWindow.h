//
// Created by mecha on 29.08.2021.
//
#pragma once

#include <GLFW/glfw3.h>
#include "../../core/Window.h"

namespace engine {

    class WindowsWindow : public Window {

    public:
        explicit WindowsWindow(const WindowProps& windowProps) : Window(windowProps) {}
        ~WindowsWindow() override;

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