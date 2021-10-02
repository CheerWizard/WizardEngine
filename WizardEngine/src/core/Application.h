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
#include "CameraController.h"

#include "../imgui/ImguiLayer.h"

#include "../graphics/GraphicsContext.h"

#include "../platform/Platform.h"

#include "../ecs/Scene.h"

#include "../graphics/io/ObjFile.h"

#include "string"
#include "vector"

#define APP Application::getInstance()
#define WINDOW APP.getWindow()
#define INPUT APP.getInput()
#define IMGUI_LAYER APP.getImGuiLayer()
#define GRAPHICS_CONTEXT APP.getGraphicsContext()

#define DEFAULT_CAMERA_NAME "camera"

namespace engine {

    enum EngineType : bool {
        ENGINE_2D = false,
        ENGINE_3D = true
    };

    class Application : public WindowCallback, KeyboardCallback, MouseCallback, CursorCallback {

    public:
        Application(const EngineType &engineType = ENGINE_3D) :
        _engineType(engineType) {}

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

    public:
        void run();
        void restart();

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
        virtual void onPrepare();
        virtual void onDestroy();
        virtual void onUpdate();

    protected:
        void pushLayer(Layer* layer);
        void pushOverlay(Layer* overlay);
        void pushLayout(Layout* imGuiLayout);
        void pushOverLayout(Layout* imGuiLayout);

        void addShader(const std::string& name, const Ref<Shader>& shader);
        void addShader(const Ref<Shader>& shader);
        ShaderError loadShader(const ShaderProps& shaderProps, VertexFormat* vertexFormat);
        ShaderError loadShader(const ShaderProps &shaderProps);
        Ref<Shader> getShader(const std::string& name);
        bool shaderExists(const std::string& name) const;

        float getAspectRatio() const;
        const uint32_t& getWindowWidth();
        const uint32_t& getWindowHeight();

        void enableDepthRendering();

        void loadTexture(const std::string &filePath);
        void loadTextureData(const void* data);

        void createCamera(const char* name = DEFAULT_CAMERA_NAME);
        void createActiveScene();

        ShapeComponent loadObj(const std::string& objName);

        void updateFboSpecification();
        void updateFboSpecification(const FramebufferSpecification &framebufferSpecification);

    private:
        void createCamera3D(const char* name);
        void createCamera3D(const char* name, const glm::vec3 &position);
        void createCamera3D(Camera3d* camera3D);

        void createCamera2D(const char* name);
        void createCamera2D(const char* name, const glm::vec3 &position);
        void createCamera2D(Camera2d* camera2D);

        void createRenderSystem();

    protected:
        Scope<Input> input;
        KeyCode closeKeyPressed;
        CameraController* cameraController; // todo move to Component.
        Scene* activeScene; // todo add Scene cache or smth similar.

    private:
        static Application* _instance;

    private:
        EngineType _engineType;
        bool _isRunning = true;
        LayerStack _layerStack;
        ImGuiLayer* _imGuiLayer;
        Scope<Window> _window;
        Scope<GraphicsContext> _graphicsContext;
        Ref<RenderSystem> _renderSystem;
        ObjFile* _objFile = nullptr;

    };

    Application* createApplication();

}