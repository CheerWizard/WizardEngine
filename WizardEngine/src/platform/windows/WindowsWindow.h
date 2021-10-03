//
// Created by mecha on 29.08.2021.
//
#pragma once

#include "../../core/Window.h"

struct GLFWwindow;
struct GLFWmonitor;

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

        uint32_t getRefreshRate() override;

    private:
        static void handleError(int error, const char* description);

    private:
        GLFWwindow* _window;

    };

}