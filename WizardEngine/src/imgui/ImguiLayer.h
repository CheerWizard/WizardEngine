//
// Created by mecha on 31.08.2021.
//

#pragma once

#include <GLFW/glfw3.h>

#include "../core/KeyCodes.h"
#include "../core/Layer.h"

#include "../core/Events.h"

namespace engine {

    class ImGuiLayer : public Layer {

    public:
        ImGuiLayer();
        ~ImGuiLayer() override = default;

    public:
        void onCreate() override;
        void onUpdate(Time deltaTime) override;
        void onDestroy() override;
        void onImGuiRender() override;

    private:
        float _time = 0.0f;

    };

}