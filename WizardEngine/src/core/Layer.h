//
// Created by mecha on 31.08.2021.
//

#pragma once

#include "Time.h"
#include "Events.h"

namespace engine {

    class Layer : public WindowCallback, KeyboardCallback, MouseCallback, CursorCallback {

    public:
        Layer(const char* tag = "Layer") : tag(tag) {}
        virtual ~Layer() = default;

    public:
        virtual void onUpdate(Time deltaTime) = 0;
        virtual void onPrepare() = 0;

    public:
        void onWindowClosed() override;
        void onWindowResized(const uint32_t &width, const uint32_t &height) override;

        void onKeyPressed(KeyCode keyCode) override;
        void onKeyHold(KeyCode keyCode) override;
        void onKeyReleased(KeyCode keyCode) override;
        void onKeyTyped(KeyCode keyCode) override;

        void onMousePressed(MouseCode mouseCode) override;
        void onMouseRelease(MouseCode mouseCode) override;
        void onMouseScrolled(double xOffset, double yOffset) override;

        void onCursorMoved(double xPos, double yPos) override;

    public:
        inline const char* getTag() const { return tag; }

    protected:
        const char* tag;

    };

}
