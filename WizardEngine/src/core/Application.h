//
// Created by mecha on 28.08.2021.
//

#pragma once

#include "Memory.h"
#include "Window.h"
#include "Events.h"
#include "LayerStack.h"
#include "Input.h"
#include "../camera/CameraController.h"
#include "FpsController.h"
#include "String.h"
#include "FileDialog.h"

#include "../ecs/Scene.h"

#include "../graphics/GraphicsContext.h"
#include "../graphics/render/RenderSystem.h"
#include "../graphics/sources/MeshSource.h"
#include "../graphics/sources/TextureSource.h"
#include "../graphics/sources/ShaderSource.h"

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

        inline const Scope<Window>& getWindow() {
            return _window;
        }

        inline const Ref<RenderSystem>& getRenderSystem() {
            return _renderSystem;
        }

        inline MeshSource& getMeshSource() {
            return _meshSource;
        }

        inline const Ref<TextureSource>& getTextureSource() {
            return _textureSource;
        }

        inline const Ref<ShaderSource>& getShaderSource() {
            return _shaderSource;
        }

        inline void bindCloseKey(const KeyCode& keyCode) {
            _closeKeyPressed = keyCode;
        }

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

    protected:
        virtual void onCreate();
        virtual void onPrepare();
        virtual void onDestroy();
        virtual void onUpdate();
        virtual WindowProps createWindowProps();

    public:
        float getAspectRatio() const;
        const uint32_t& getWindowWidth();
        const uint32_t& getWindowHeight();
        void createActiveScene();
        uint32_t getRefreshRate();
        void* getNativeWindow();
        void setWindowIcon(const std::string &filePath);
        Ref<FileDialog> createFileDialog();

    protected:
        void pushFront(Layer* layer);
        void pushBack(Layer* overlay);

    private:
        void restart();

        void createGraphics();
        void createSources();
        void createFrameSpecs();

    public:
        Scope<Input> input; // input controller!
        Ref<Scene> activeScene; // todo add SceneSource and cache multiple Scenes.
        Ref<FrameController> activeFrameController; // controller for "active" scene frame!
        FpsController fpsController;

    private:
        EngineType _engineType;
        bool _isRunning = true;
        KeyCode _closeKeyPressed;

        LayerStack _layerStack;
        Scope<Window> _window;
        Scope<GraphicsContext> _graphicsContext;
        Ref<RenderSystem> _renderSystem;
        // data sources
        Ref<GraphicsFactory> _graphicsFactory;
        MeshSource _meshSource;
        Ref<TextureSource> _textureSource;
        Ref<ShaderSource> _shaderSource;
    };

    Application* createApplication();

}