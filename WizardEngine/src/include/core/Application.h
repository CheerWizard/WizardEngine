//
// Created by mecha on 28.08.2021.
//

#pragma once

#include "Memory.h"
#include "Events.h"
#include "LayerStack.h"
#include "FpsController.h"
#include "String.h"

#include <platform/core/Window.h>
#include <platform/core/Input.h>
#include <platform/tools/FileDialog.h>
#include <platform/graphics/GraphicsInitializer.h>

#include <graphics/core/RenderSystem.h>
#include <graphics/core/sources/MeshSource.h>
#include <graphics/core/sources/TextureSource.h>
#include <graphics/core/sources/ShaderSource.h>

#include <scripting/ScriptSystem.h>

#define DEFAULT_CAMERA_NAME "camera"

namespace engine {

    enum EngineType : bool {
        ENGINE_2D = false,
        ENGINE_3D = true
    };

    class Application {

    public:
        Application(const EngineType &engineType = ENGINE_3D) :
        _engineType(engineType) {}

        virtual ~Application() = default;

    public:
        void run();
        void shutdown();

    public:
        inline const Scope<Window>& getWindow() {
            return _window;
        }

    public:
        void onWindowClosed();
        void onWindowResized(const uint32_t &width, const uint32_t &height);

        void onKeyPressed(KeyCode keyCode);
        void onKeyHold(KeyCode keyCode);
        void onKeyReleased(KeyCode keyCode);
        void onKeyTyped(KeyCode keyCode);

        void onMousePressed(MouseCode mouseCode);
        void onMouseRelease(MouseCode mouseCode);
        void onMouseScrolled(double xOffset, double yOffset);

        void onCursorMoved(double xPos, double yPos);

    protected:
        virtual void onCreate();
        virtual void onPrepare();
        virtual void onDestroy();
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

        void setSkybox(const Entity& skybox);

    protected:
        void pushFront(Layer* layer);
        void pushBack(Layer* overlay);

    private:
        void restart();
        void onUpdate();
        void updateRuntime(Time dt);

        void createGraphics();
        void createFrameSpecs();
        void createScripting();

    public:
        Scope<Input> input;
        Ref<Scene> activeScene;
        Ref<FrameBuffer> activeFrame;
        FpsController fpsController;

    private:
        EngineType _engineType;
        bool _isRunning = true;
        // core systems
        LayerStack _layerStack;
        Scope<Window> _window;
        // graphics system
        Scope<RenderSystem> _renderSystem;
        // scripting system
        Scope<ScriptSystem> _scriptSystem;

    };

    Application* createApplication();

}