//
// Created by mecha on 31.08.2021.
//

#pragma once

#include <string>

#include "Time.h"
#include "Logger.h"

#include "Events.h"

namespace engine {

    class Layer : public WindowCallback, KeyboardCallback, MouseCallback, CursorCallback {

    public:

        explicit Layer(const std::string& tag = "Layer") : tag(tag) {}

        virtual ~Layer() {
            onDestroy();
        }

    public:
        virtual void onCreate();
        virtual void onUpdate(Time deltaTime);
        virtual void onDestroy();
        virtual void onImGuiRender();

    public:
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

    public:
        const std::string& getName() const { return tag; }

    protected:
        std::string tag;

    };

}
