//
// Created by mecha on 31.08.2021.
//

#pragma once

#include <core/Application.h>
#include <core/Layer.h>

struct ImFont;

namespace engine::gui {

    struct ENGINE_API ImGuiLayerProps {
        uint32_t width, height;
        const char* tag = "ImGuiLayer";
    };

    class ENGINE_API ImGuiLayer : public core::Layer {

    public:
        ImGuiLayer(Application* app, const ImGuiLayerProps &props) :
                   app(app),
                   props(props),
                   Layer(props.tag) {
            create();
        }

        ~ImGuiLayer() override {
            destroy();
        }

    public:
        void onUpdate(Time dt) override;
        void onPrepare() override;

    protected:
        // method in which you can render ImGui widgets.
        virtual void onRender(Time dt) = 0;

    public:
        static void setDarkTheme();
        static void setDarkThemeV2();
        static void setDockSpaceOption(const bool &isFullscreen);
        static void openDockSpace();
        static void hideDockSpace();
        static void toggleDockSpace();

        void onKeyPressed(event::KeyCode keyCode) override;
        void onKeyHold(event::KeyCode keyCode) override;
        void onKeyReleased(event::KeyCode keyCode) override;
        void onKeyTyped(event::KeyCode keyCode) override;
        void onMouseScrolled(double xOffset, double yOffset) override;
        void onMousePressed(event::MouseCode mouseCode) override;
        void onMouseRelease(event::MouseCode mouseCode) override;
        void onCursorMoved(double xPos, double yPos) override;

        void onWindowClosed() override;

        void onWindowResized(const uint32_t &width, const uint32_t &height) override;

    private:
        void create();
        void destroy();
        static void onBeginFrame();
        void onEndFrame() const;

    private:
        static void beginDockSpace();
        static void setDockSpace();
        static void endDockSpace();

    protected:
        core::Application* app = nullptr;
        ImGuiLayerProps props;
        static bool isDockSpaceOpened;
        static bool isFullScreen;
        static int windowFlags;
        static int dockSpaceFlags;

        ImFont* boldFont = nullptr;
        ImFont* regularFont = nullptr;
        ImFont* resizableFont = nullptr;
    };

}