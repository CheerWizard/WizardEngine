//
// Created by mecha on 29.08.2021.
//
#pragma once

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "../../core/Window.h"
#include "../../core/Memory.h"
#include "../../core/Logger.h"
#include "../../core/Assert.h"

#include "../../events/KeyEvent.h"
#include "../../events/MouseEvent.h"
#include "../../events/WindowEvent.h"

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