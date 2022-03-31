//
// Created by mecha on 29.08.2021.
//
#pragma once

#include <core/Events.h>
#include <core/Memory.h>

#include <string>

#define DEFAULT_WINDOW_WIDTH 1024
#define DEFAULT_WINDOW_HEIGHT 768

#define WINDOW_CALLBACK(window, app) window->setWindowCallback<Application>(createRef<WindowCallback<Application>>(app))
#define MOUSE_CALLBACK(window, app) window->setMouseCallback<Application>(createRef<MouseCallback<Application>>(app))
#define KEYBOARD_CALLBACK(window, app) window->setKeyboardCallback<Application>(createRef<KeyboardCallback<Application>>(app))
#define CURSOR_CALLBACK(window, app) window->setCursorCallback<Application>(createRef<CursorCallback<Application>>(app))

namespace engine {

    class Application;

    struct WindowProps {
        std::string title;
        uint32_t width;
        uint32_t height;
        bool vSyncEnabled;
        uint32_t sampleSize;

        Ref<WindowCallback<Application>> windowCallback = nullptr;
        Ref<KeyboardCallback<Application>> keyboardCallback = nullptr;
        Ref<MouseCallback<Application>> mouseCallback = nullptr;
        Ref<CursorCallback<Application>> cursorCallback = nullptr;

        WindowProps(
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
        Window(const WindowProps &windowProps = WindowProps()) : windowProps(windowProps) {
            create();
        }
        ~Window() {
            destroy();
        }

    public:
        [[nodiscard]] inline void* getNativeWindow() const {
            return window;
        }

        void onPrepare();
        void onUpdate();
        void onClose();

        uint32_t getRefreshRate();

        void setWindowIcon(const std::string &filePath);
        void disableFullScreen();
        void enableFullScreen();
        void setSampleSize(const uint32_t &size);
        void setPosition(const uint32_t &x, const uint32_t &y);

    public:
        void removeCallbacks();
        void toggleFullScreen();
        void setInCenter();

    public:
        void enableVSync();
        void disableVSync();

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

        inline const uint32_t& getSampleSize() const {
            return windowProps.sampleSize;
        }

        inline float getAspectRatio() const {
            return (float) getWidth() / (float) getHeight();
        }

        inline bool isVSyncEnabled() const {
            return windowProps.vSyncEnabled;
        }

        template<typename T>
        inline void setWindowCallback(const Ref<WindowCallback<T>>& windowCallback) {
            windowProps.windowCallback = windowCallback;
        }

        inline void removeWindowCallback() {
            windowProps.windowCallback = nullptr;
        }

        template<typename T>
        inline void setKeyboardCallback(const Ref<KeyboardCallback<T>>& keyboardCallback) {
            windowProps.keyboardCallback = keyboardCallback;
        }

        inline void removeKeyboardCallback() {
            windowProps.keyboardCallback = nullptr;
        }

        template<typename T>
        inline void setMouseCallback(const Ref<MouseCallback<T>>& mouseCallback) {
            windowProps.mouseCallback = mouseCallback;
        }

        inline void removeMouseCallback() {
            windowProps.mouseCallback = nullptr;
        }

        template<typename T>
        inline void setCursorCallback(const Ref<CursorCallback<T>>&cursorCallback) {
            windowProps.cursorCallback = cursorCallback;
        }

        inline void removeCursorCallback() {
            windowProps.cursorCallback = nullptr;
        }

    private:
        void create();
        void destroy();
        static void handleError(int error, const char *description);
        void onWindowResized(const uint32_t& width, const uint32_t& height);

    protected:
        void* window;
        WindowProps windowProps;
        bool isInitialized = false;
        bool isFullScreen = false;

    };

}