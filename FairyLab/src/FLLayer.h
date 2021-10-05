//
// Created by mecha on 04.10.2021.
//

#pragma once

#include "imgui/ImguiLayer.h"
#include "imgui/ImageLayout.h"
#include "imgui/EntityLayout.h"

namespace fairy {

    class FLLayer : public engine::ImGuiLayer {

    public:
        FLLayer(engine::Application* app, const engine::ImGuiLayerProps &props) : engine::ImGuiLayer(app, props) {
            create();
        }

        ~FLLayer() override {
            destroy();
        }

    public:
        void onPrepare() override;

        void onUpdate(engine::Time dt) override;

        void onKeyPressed(engine::KeyCode keyCode) override;

    protected:
        void onRender(engine::Time dt) override;

    private:
        void create();
        void destroy();

    private:
        engine::ImageLayout* _scenePanel;
        engine::EntityLayout* _entityPanel;
        engine::Entity carEntity;

    };

}