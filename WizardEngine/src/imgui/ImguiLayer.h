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
        float _time = 0.0f;

    };

}