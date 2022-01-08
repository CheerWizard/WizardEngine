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
        uint32_t getRefreshRate() override;

        void setWindowIcon(const std::string &filePath) override;

        void enableFullScreen() override;
        void disableFullScreen() override;
        void setSampleSize(const uint32_t &size) override;
        void setPosition(const uint32_t &x, const uint32_t &y) override;

    private:
        static void handleError(int error, const char* description);

    private:
        void create();
        void destroy();

        void onWindowResized(const uint32_t &width, const uint32_t &height);

    };

}