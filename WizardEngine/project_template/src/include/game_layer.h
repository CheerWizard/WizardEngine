//
// Created by mecha on 11.04.2022.
//

#pragma once

#include <core/Application.h>

namespace game {

    class GameLayer : public Layer {

    public:
        GameLayer();
        ~GameLayer() override;

    public:
        void onPrepare() override;
        void onUpdate(Time deltaTime) override;

        void onWindowResized(const uint32_t &width, const uint32_t &height) override;

        void onKeyPressed(KeyCode keyCode) override;
        void onKeyHold(KeyCode keyCode) override;
        void onKeyReleased(KeyCode keyCode) override;
        void onKeyTyped(KeyCode keyCode) override;

    public:
        void onWindowClosed() override;
        void onMousePressed(MouseCode mouseCode) override;
        void onMouseRelease(MouseCode mouseCode) override;
        void onMouseScrolled(double xOffset, double yOffset) override;
        void onCursorMoved(double xPos, double yPos) override;
        void onVisualDraw(Time dt) override;
    };

}
