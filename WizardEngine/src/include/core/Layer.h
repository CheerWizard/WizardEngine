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
        virtual void onWindowClosed();
        virtual void onWindowResized(const uint32_t &width, const uint32_t &height);

        virtual void onKeyPressed(event::KeyCode keyCode);
        virtual void onKeyHold(event::KeyCode keyCode);
        virtual void onKeyReleased(event::KeyCode keyCode);
        virtual void onKeyTyped(event::KeyCode keyCode);

        virtual void onMousePressed(event::MouseCode mouseCode);
        virtual void onMouseRelease(event::MouseCode mouseCode);
        virtual void onMouseScrolled(double xOffset, double yOffset);

        virtual void onCursorMoved(double xPos, double yPos);

    public:
        [[nodiscard]] inline const char* getTag() const { return tag; }

    protected:
        const char* tag;

    };

}
