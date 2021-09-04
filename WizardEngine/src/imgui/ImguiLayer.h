//
// Created by mecha on 31.08.2021.
//

#pragma once

#include "imgui.h"
#include <backends/imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

#include "../core/KeyCodes.h"
#include "../core/Layer.h"

#include "../events/Event.h"

namespace engine {

    class ImGuiLayer : public Layer {

    public:
        ImGuiLayer();
        ~ImGuiLayer();

    public:
        void onEvent(Event& e) override;
        void onAttach() override;
        void onDetach() override;
        void onUpdate(Time deltaTime) override;

    private:
        float _time = 0.0f;

    };

}