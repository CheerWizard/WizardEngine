//
// Created by mecha on 31.08.2021.
//

#pragma once

#include "../core/Application.h"
#include "../core/Layer.h"

namespace engine {

    struct ImGuiLayerProps {
        uint32_t width, height;
        const char* tag = "ImGuiLayer";
    };

    class ImGuiLayer : public Layer {

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
        static void setDockSpaceOption(const bool &isFullscreen);
        static void openDockSpace();
        static void hideDockSpace();
        static void toggleDockSpace();

    private:
        void create();
        void destroy();
        void onBeginFrame();
        void onEndFrame();

    private:
        static void beginDockSpace();
        static void setDockSpace();
        static void endDockSpace();

    protected:
        Application* app = nullptr;
        ImGuiLayerProps props;
        static bool isFullScreen, isDockSpaceOpened;
        static int windowFlags, dockSpaceFlags;

    };

}