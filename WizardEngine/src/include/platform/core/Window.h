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

    struct ENGINE_API WindowProps {
        std::string title;
        int width;
        int height;
        bool vSyncEnabled;
        int sampleSize;
        bool fullscreen;
        bool dockspace = false;

        WindowProps(
                const std::string& title = "Wizard Engine",
                int width = DEFAULT_WINDOW_WIDTH,
                int height = DEFAULT_WINDOW_HEIGHT,
                bool vSyncEnabled = false,
                uint32_t sampleSize = 4,
                bool fullscreen = false
        ) : title(title), width(width), height(height),
        vSyncEnabled(vSyncEnabled), sampleSize(sampleSize), fullscreen(fullscreen) {}
    };

    class ENGINE_API Window {

    public:
        Window(const WindowProps &windowProps = WindowProps());
        ~Window();

    public:
        [[nodiscard]] inline void* getNativeWindow() const {
            return window;
        }

        void onPrepare();
        void onUpdate();
        void onClose();

        static int getRefreshRate();

        void setWindowIcon(const std::string &filePath);
        void disableFullScreen();
        void enableFullScreen();
        void setSampleSize(int size);
        void setPosition(int x, int y);
        void getPosition(int& x, int& y);

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

        [[nodiscard]] inline int getWidth() const {
            return windowProps.width;
        }

        [[nodiscard]] inline int getHeight() const {
            return windowProps.height;
        }

        [[nodiscard]] inline int getSampleSize() const {
            return windowProps.sampleSize;
        }

        [[nodiscard]] inline float getAspectRatio() const {
            return (float) getWidth() / (float) getHeight();
        }

        [[nodiscard]] inline bool isVSyncEnabled() const {
            return windowProps.vSyncEnabled;
        }

    private:
        static void handleError(int error, const char *description);
        static void onWindowResized(int width, int height);

    protected:
        void* window;
        WindowProps windowProps;
        bool isInitialized = false;
        bool isFullScreen = false;
        u32 xPos = 0;
        u32 yPos = 0;
    };

}