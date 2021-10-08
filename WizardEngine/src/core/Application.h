//
// Created by mecha on 28.08.2021.
//

#pragma once

#include "Memory.h"
#include "Window.h"
#include "Events.h"
#include "LayerStack.h"
#include "Input.h"
#include "CameraController.h"
#include "Timer.h"
#include "String.h"

#include "../ecs/Scene.h"

#include "../graphics/GraphicsContext.h"
#include "../graphics/render/RenderSystem.h"
#include "../graphics/io/ObjFile.h"
#include "../graphics/geometry/Shapes.h"

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
        void run();

    public:
        inline const Ref<GraphicsFactory>& getGraphicsFactory() {
            return _graphicsFactory;
        }

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
        virtual WindowProps createWindowProps();

    public:
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
        void createCamera(const char* name = DEFAULT_CAMERA_NAME);
        void createActiveScene();
        void updateFboSpecification();
        void updateFboSpecification(const FramebufferSpecification &framebufferSpecification);
        void setPolygonMode(const PolygonMode &polygonMode);
        uint32_t getRefreshRate();
        void* getNativeWindow();
        void enableDisplay();
        void disableDisplay();
        Ref<MeshComponent> loadObj(const std::string& objName);
        void loadTexture(const std::string &fileName);

    protected:
        void pushFront(Layer* layer);
        void pushBack(Layer* overlay);

    private:
        void restart();

        void createCamera3D(const char* name);
        void createCamera3D(const char* name, const glm::vec3 &position);
        void createCamera3D(Camera3d* camera3D);

        void createCamera2D(const char* name);
        void createCamera2D(const char* name, const glm::vec3 &position);
        void createCamera2D(Camera2d* camera2D);

        void createRenderSystem();

    public:
        Scope<Input> input;
        KeyCode closeKeyPressed;
        Ref<CameraController> sceneCameraController; // todo move to Component.
        Ref<Scene> activeScene; // todo add Scene cache or smth similar.
        Timer fpsTimer;
        ObjFile objFile;

    private:
        EngineType _engineType;
        bool _isRunning = true;
        LayerStack _layerStack;
        Scope<Window> _window;
        Scope<GraphicsContext> _graphicsContext;
        Ref<RenderSystem> _renderSystem;
        bool _isDisplayEnabled = true;
        Ref<GraphicsFactory> _graphicsFactory;

    };

    Application* createApplication();

}