//
// Created by mecha on 29.08.2021.
//
#pragma once

#include <core/Memory.h>
#include <event/Events.h>
#include <string>

#define DEFAULT_WINDOW_WIDTH 1024
#define DEFAULT_WINDOW_HEIGHT 768

namespace engine::core {

    struct WindowProps {
        std::string title;
        uint32_t width;
        uint32_t height;
        bool vSyncEnabled;
        uint32_t sampleSize;

        WindowProps(
                const std::string& title = "Wizard Engine",
                uint32_t width = DEFAULT_WINDOW_WIDTH,
                uint32_t height = DEFAULT_WINDOW_HEIGHT,
                bool vSyncEnabled = false,
                uint32_t sampleSize = 4
        ) : title(title), width(width), height(height), vSyncEnabled(vSyncEnabled), sampleSize(sampleSize) {}
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

        static uint32_t getRefreshRate();

        void setWindowIcon(const std::string &filePath);
        void disableFullScreen();
        void enableFullScreen();
        void setSampleSize(const uint32_t &size);
        void setPosition(const uint32_t &x, const uint32_t &y);
        void getPosition(float x, float y);

    public:
        void toggleFullScreen();
        void setInCenter();

    public:
        void enableVSync();
        void disableVSync();

    public:
        static void loadGamepadMappings(const char* filePath);

    public:
        inline const WindowProps& getWindowProps() {
            return windowProps;
        }

        [[nodiscard]] inline const uint32_t& getWidth() const {
            return windowProps.width;
        }

        [[nodiscard]] inline const uint32_t& getHeight() const {
            return windowProps.height;
        }

        [[nodiscard]] inline const uint32_t& getSampleSize() const {
            return windowProps.sampleSize;
        }

        [[nodiscard]] inline float getAspectRatio() const {
            return (float) getWidth() / (float) getHeight();
        }

        [[nodiscard]] inline bool isVSyncEnabled() const {
            return windowProps.vSyncEnabled;
        }

    private:
        void create();
        void destroy();
        static void handleError(int error, const char *description);
        static void onWindowResized(const uint32_t& width, const uint32_t& height);

    protected:
        void* window;
        WindowProps windowProps;
        bool isInitialized = false;
        bool isFullScreen = false;
        u32 xPos = 0;
        u32 yPos = 0;
    };

}