//
// Created by mecha on 31.08.2021.
//

#pragma once

#include <time/Time.h>
#include <event/Events.h>

namespace engine::core {

    class Layer {

    public:
        Layer(const char* tag = "Layer") : tag(tag) {}
        virtual ~Layer() = default;

    public:
        virtual void onUpdate(time::Time deltaTime) = 0;
        virtual void onPrepare() = 0;

    public:
        virtual void onWindowClosed() = 0;
        virtual void onWindowResized(const uint32_t &width, const uint32_t &height) = 0;

        virtual void onKeyPressed(event::KeyCode keyCode) = 0;
        virtual void onKeyHold(event::KeyCode keyCode) = 0;
        virtual void onKeyReleased(event::KeyCode keyCode) = 0;
        virtual void onKeyTyped(event::KeyCode keyCode) = 0;

        virtual void onMousePressed(event::MouseCode mouseCode) = 0;
        virtual void onMouseRelease(event::MouseCode mouseCode) = 0;
        virtual void onMouseScrolled(double xOffset, double yOffset) = 0;

        virtual void onCursorMoved(double xPos, double yPos) = 0;

        virtual void onVisualDraw(time::Time dt) = 0;

    public:
        [[nodiscard]] inline const char* getTag() const { return tag; }

    protected:
        const char* tag;

    };
}
