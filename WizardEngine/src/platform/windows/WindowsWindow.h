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
        WindowsWindow(const WindowProps& windowProps = WindowProps()) : Window(windowProps) {
            create();
        }

        ~WindowsWindow() override {
            destroy();
        }

    public:
        void onPrepare() override;
        void onUpdate() override;
        void onClose() override;

        void enableVSync() override;
        void disableVSync() override;

    public:
        void* getNativeWindow() const override { return _window; }

        uint32_t getRefreshRate() override;

        void setWindowIcon(const std::string &filePath) override;

    private:
        static void handleError(int error, const char* description);

    private:
        void create();
        void destroy();

    private:
        GLFWwindow* _window;

    };

}