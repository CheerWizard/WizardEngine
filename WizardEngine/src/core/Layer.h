//
// Created by mecha on 31.08.2021.
//

#pragma once

#include "Time.h"
#include "Events.h"

namespace engine {

    class Layer : public WindowCallback, KeyboardCallback, MouseCallback, CursorCallback {

    public:

        Layer(const char* tag = "Layer") : tag(tag) {
            onCreate();
        }

        virtual ~Layer() {
            onDestroy();
        }

    public:
        virtual void onUpdate(Time deltaTime);

    protected:
        virtual void onCreate();
        virtual void onDestroy();

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
        inline const char* getTag() const { return tag; }

    protected:
        const char* tag;

    };

}
