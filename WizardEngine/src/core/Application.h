//
// Created by mecha on 28.08.2021.
//

#pragma once

#include "Memory.h"
#include "Window.h"
#include "Logger.h"
#include "Events.h"
#include "LayerStack.h"
#include "Assert.h"
#include "Input.h"

namespace engine {

    class Application : public WindowCallback, KeyboardCallback, MouseCallback, CursorCallback {

    public:
        Application() = default;
        virtual ~Application() = default;

    public:
        static inline Application& getInstance() {
            return *_instance;
        }

    public:
        inline Window& getWindow() {
            return *window;
        }

    public:
        inline Input& getInput() {
            return *input;
        }

    public:
        void run();

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

    protected:
        virtual void onCreate();
        virtual void onDestroy();
        virtual void onUpdate();

    protected:
        void pushLayer(Layer* layer);
        void pushOverlay(Layer* overlay);

    private:
        static Application* _instance;

    private:
        bool _isRunning = true;
        LayerStack _layerStack;

    protected:
        Scope<Window> window;
        Scope<Input> input;

    };

    Application* createApplication();

}