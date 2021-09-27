//
// Created by mecha on 29.08.2021.
//
#pragma once

#include <GLFW/glfw3.h>

#include "../../core/Window.h"

namespace engine {

    class WindowsWindow : public Window {

    public:
        explicit WindowsWindow(const WindowProps& windowProps) : Window(windowProps) {

        }
        ~WindowsWindow() override = default;

    public:
        void onUpdate() override;
        void onCreate() override;
        void onClose() override;
        void onDestroy() override;
        void onPrepare() override;

        void enableVSync() override;
        void disableVSync() override;

    public:
        void* getNativeWindow() const override { return _window; }

    private:
        static void handleError(int error, const char* description);

    private:
        GLFWwindow* _window;

    };

}