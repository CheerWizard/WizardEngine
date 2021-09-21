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
#include "../graphics/shader/ShaderCache.h"

#include "string"
#include "vector"

#define APP Application::getInstance()
#define WINDOW APP.getWindow()
#define INPUT APP.getInput()
#define IMGUI_LAYER APP.getImGuiLayer()
#define GRAPHICS_CONTEXT APP.getGraphicsContext()
#define SHADERS APP.getShaderCache()
#define GRAPHICS_OBJECTS APP.getGraphicsObjectCache()

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
        inline const Scope<Window>& getWindow() const {
            return _window;
        }

        inline const Scope<Input>& getInput() const {
            return input;
        }

        inline ImGuiLayer& getImGuiLayer() const {
            return *_imGuiLayer;
        }

        inline const Scope<GraphicsContext>& getGraphicsContext() const {
            return _graphicsContext;
        }

        inline ShaderCache& getShaderCache() const {
            return *_shaderCache;
        }

        inline GraphicsObjectCache& getGraphicsObjectCache() const {
            return *_graphicsObjectCache;
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

        void addShader(const std::string& name, const Ref<Shader>& shader);
        void addShader(const Ref<Shader>& shader);
        Ref<Shader> loadShader(const std::string& filepath);
        Ref<Shader> loadShader(const std::string& name, const std::string& filepath);
        Ref<Shader> getShader(const std::string& name);
        bool shaderExists(const std::string& name) const;

        void loadVertices(const std::string &shaderName,
                          const uint32_t &vertexStart,
                          float* vertices);
        void loadIndices(const std::string &shaderName,
                         const uint32_t &indexStart,
                         uint32_t* indices);

        void loadObject(const std::string &shaderName,
                        const Ref<GraphicsObject>& graphicsObject);

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
        Ref<Renderer> _renderer;
        ShaderCache* _shaderCache;
        GraphicsObjectCache* _graphicsObjectCache;

    };

    Application* createApplication();

}