//
// Created by mecha on 31.08.2021.
//

#pragma once

#include "Time.h"
#include "Events.h"

namespace engine {

    class Layer {

    public:
        Layer(const char* tag = "Layer") : tag(tag) {}
        virtual ~Layer() = default;

    public:
        virtual void onUpdate(Time deltaTime) = 0;
        virtual void onPrepare() = 0;

    public:
        virtual void onWindowClosed();
        virtual void onWindowResized(const uint32_t &width, const uint32_t &height);

        virtual void onKeyPressed(KeyCode keyCode);
        virtual void onKeyHold(KeyCode keyCode);
        virtual void onKeyReleased(KeyCode keyCode);
        virtual void onKeyTyped(KeyCode keyCode);

        virtual void onMousePressed(MouseCode mouseCode);
        virtual void onMouseRelease(MouseCode mouseCode);
        virtual void onMouseScrolled(double xOffset, double yOffset);

        virtual void onCursorMoved(double xPos, double yPos);

    public:
        [[nodiscard]] inline const char* getTag() const { return tag; }

    protected:
        const char* tag;

    };

}
