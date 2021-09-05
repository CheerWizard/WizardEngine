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

#include "../imgui/ImguiLayer.h"

#include "../graphics/GraphicsContext.h"

#include "../platform/Platform.h"

#define APP Application::getInstance()
#define WINDOW APP.getWindow()
#define INPUT APP.getInput()
#define IMGUI_LAYER APP.getImGuiLayer()
#define GRAPHICS_CONTEXT APP.getGraphicsContext()

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
            return *_window;
        }

    public:
        inline Input& getInput() {
            return *input;
        }

    public:
        inline ImGuiLayer& getImGuiLayer() {
            return *_imGuiLayer;
        }

    public:
        inline GraphicsContext& getGraphicsContext() {
            return *_graphicsContext;
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
        void pushLayout(Layout* imGuiLayout);
        void pushOverLayout(Layout* imGuiLayout);

    protected:
        Scope<Input> input;

    private:
        static Application* _instance;

    private:
        bool _isRunning = true;
        LayerStack _layerStack;
        ImGuiLayer* _imGuiLayer;
        Scope<Window> _window;
        Scope<GraphicsContext> _graphicsContext;
        Renderer* _renderer;

    };

    Application* createApplication();

}