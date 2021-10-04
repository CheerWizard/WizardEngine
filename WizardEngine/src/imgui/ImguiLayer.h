//
// Created by mecha on 31.08.2021.
//

#pragma once

#include "../core/Layout.h"
#include "../core/LayoutStack.h"

namespace engine {

    class ImGuiLayer : public Layer {

    public:
        ImGuiLayer(void* nativeWindow,
                   const uint32_t &width,
                   const uint32_t &height,
                   const char* tag = "ImGuiLayer") :
        Layer(tag),
        _width(width),
        _height(height)
        {
            onCreate(nativeWindow);
        }

        ~ImGuiLayer() override {
            onDestroy();
        }

    public:
        void onUpdate(Time deltaTime) override;

    protected:
        void onCreate(void* nativeWindow);
        void onDestroy() override;

    public:
        static void setDarkTheme();
        static void setDockSpaceOption(const bool &isFullscreen);
        static void openDockSpace();
        static void hideDockSpace();
        static void toggleDockSpace();

    public:
        void pushLayout(Layout* imGuiLayout);
        void pushOverLayout(Layout* imGuiLayout);
        void popLayout(Layout* imGuiLayout);
        void popOverLayout(Layout* imGuiLayout);

    public:
        void onWindowClosed() override;
        void onWindowResized(unsigned int width, unsigned int height) override;

        void onKeyPressed(KeyCode keyCode) override;
        void onKeyHold(KeyCode keyCode) override;
        void onKeyReleased(KeyCode keyCode) override;
        void onKeyTyped(KeyCode keyCode) override;

        void onMousePressed(MouseCode mouseCode) override;
        void onMouseRelease(MouseCode mouseCode) override;
        void onMouseScrolled(double xOffset, double yOffset) override;

        void onCursorMoved(double xPos, double yPos) override;

    private:
        void onBeginFrame();
        void onEndFrame();

    private:
        static void beginDockSpace();
        static void setDockSpace();
        static void endDockSpace();

    private:
        LayoutStack _layoutStack;

    private:
        static bool _isFullScreen, _isDockSpaceOpened;
        static int _windowFlags, _dockSpaceFlags;

    private:
        uint32_t _width, _height;

    };

}