//
// Created by mecha on 31.08.2021.
//

#pragma once

#include "../core/Layer.h"
#include "../events/Event.h"

namespace engine {

    class ImGuiLayer : public Layer {

    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

    public:
        void onEvent(Event& e) override;
        virtual void onUpdate();

    };

}