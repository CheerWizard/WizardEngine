//
// Created by mecha on 29.08.2021.
//
#pragma once

#include "Events.h"

#include <string>

#define DEFAULT_WINDOW_WIDTH 1024
#define DEFAULT_WINDOW_HEIGHT 768

namespace engine {

    struct WindowProps {
        std::string title;
        uint32_t width;
        uint32_t height;
        bool vSyncEnabled;
        uint32_t sampleSize;

        WindowCallback* windowCallback = nullptr;
        KeyboardCallback* keyboardCallback = nullptr;
        MouseCallback* mouseCallback = nullptr;
        CursorCallback* cursorCallback = nullptr;

        explicit WindowProps(
                const std::string& title = "Wizard Engine",
                uint32_t width = DEFAULT_WINDOW_WIDTH,
                uint32_t height = DEFAULT_WINDOW_HEIGHT,
                bool vSyncEnabled = false,
                uint32_t sampleSize = 4
        ) : title(title), width(width), height(height), vSyncEnabled(vSyncEnabled), sampleSize(sampleSize) {
        }

    };

    class Window {

    public:
        Window(const WindowProps &windowProps = WindowProps()) : windowProps(windowProps) {}

        virtual ~Window() = default;

    public:
        [[nodiscard]] virtual void* getNativeWindow() const {
            return window;
        }

        virtual void onPrepare() = 0;
        virtual void onUpdate() = 0;
        virtual void onClose() = 0;

        virtual uint32_t getRefreshRate() = 0;

        virtual void setWindowIcon(const std::string &filePath) = 0;
        virtual void disableFullScreen() = 0;
        virtual void enableFullScreen() = 0;
        virtual void setSampleSize(const uint32_t &size) = 0;
        virtual void setPosition(const uint32_t &x, const uint32_t &y) = 0;

    public:
        void removeCallbacks();
        void toggleFullScreen();
        void setInCenter();

    public:
        virtual void enableVSync() {
            windowProps.vSyncEnabled = true;
        }
        virtual void disableVSync() {
            windowProps.vSyncEnabled = false;
        }

    public:
        inline const WindowProps& getWindowProps() {
            return windowProps;
        }

        inline const uint32_t& getWidth() const {
            return windowProps.width;
        }

        inline const uint32_t& getHeight() const {
            return windowProps.height;
        }

        inline float getAspectRatio() const {
            return (float) getWidth() / (float) getHeight();
        }

        inline bool isVSyncEnabled() const {
            return windowProps.vSyncEnabled;
        }

        inline void setWindowCallback(WindowCallback *windowCallback) {
            windowProps.windowCallback = windowCallback;
        }

        inline void removeWindowCallback() {
            windowProps.windowCallback = nullptr;
        }

        inline void setKeyboardCallback(KeyboardCallback *keyboardCallback) {
            windowProps.keyboardCallback = keyboardCallback;
        }

        inline void removeKeyboardCallback() {
            windowProps.keyboardCallback = nullptr;
        }

        inline void setMouseCallback(MouseCallback *mouseCallback) {
            windowProps.mouseCallback = mouseCallback;
        }

        inline void removeMouseCallback() {
            windowProps.mouseCallback = nullptr;
        }

        inline void setCursorCallback(CursorCallback *cursorCallback) {
            windowProps.cursorCallback = cursorCallback;
        }

        inline void removeCursorCallback() {
            windowProps.cursorCallback = nullptr;
        }

    protected:
        void* window;
        WindowProps windowProps;
        bool isInitialized = false;
        bool isFullScreen = false;

    };

}