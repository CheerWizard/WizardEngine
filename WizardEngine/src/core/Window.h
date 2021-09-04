//
// Created by mecha on 29.08.2021.
//
#pragma once

#include "../events/Event.h"
#include "Memory.h"

namespace engine {

    class WindowCallback {

    public:
        virtual void onWindowClosed() = 0;
        virtual void onWindowResized(unsigned int width, unsigned int height) = 0;

    };

    struct WindowProps {
        std::string title;
        uint32_t width;
        uint32_t height;
        bool vSyncEnabled;

        WindowCallback* windowCallback = nullptr;
        EventCallback* eventCallback = nullptr;

        explicit WindowProps(
                const std::string& title = "Wizard Engine",
                uint32_t width = 800,
                uint32_t height = 600,
                bool vSyncEnabled = false)
        : title(title), width(width), height(height), vSyncEnabled(vSyncEnabled) {

        }

    };

    class Window {

    public:
        explicit Window(const WindowProps& windowProps) : windowProps(windowProps) {

        };
        virtual ~Window() = default;

    public:
        static Scope<Window> newInstance(const WindowProps& props = WindowProps());

    public:
        virtual void* getNativeWindow() const = 0;

        virtual void onCreate() = 0;
        virtual void onUpdate() = 0;
        virtual void onClose() = 0;

        virtual void onDestroy() {
            removeWindowCallback();
            removeEventCallback();
        }

    public:
        virtual void enableVSync() {
            windowProps.vSyncEnabled = true;
        }
        virtual void disableVSync() {
            windowProps.vSyncEnabled = false;
        }

    public:

        inline uint32_t getWidth() const {
            return windowProps.width;
        }

        inline uint32_t getHeight() const {
            return windowProps.height;
        }

        inline bool isVSyncEnabled() const {
            return windowProps.vSyncEnabled;
        }

        inline void setEventCallback(EventCallback *eventCallback) {
            windowProps.eventCallback = eventCallback;
        }

        inline void removeEventCallback() {
            windowProps.eventCallback = nullptr;
        }

        inline void setWindowCallback(WindowCallback *windowCallback) {
            windowProps.windowCallback = windowCallback;
        }

        inline void removeWindowCallback() {
            windowProps.windowCallback = nullptr;
        }

    protected:
        WindowProps windowProps;
        bool isInitialized = false;

    };

}