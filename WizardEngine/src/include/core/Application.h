//
// Created by mecha on 28.08.2021.
//

#pragma once

#include <core/String.h>
#include <core/LayerStack.h>
#include <core/Memory.h>
#include <event/Events.h>
#include <time/FpsController.h>

#include <platform/core/Window.h>
#include <platform/core/Input.h>
#include <platform/tools/FileDialog.h>
#include <platform/graphics/GraphicsInitializer.h>

#include <graphics/core/RenderSystem.h>
#include <graphics/core/sources/MeshSource.h>
#include <graphics/core/sources/TextureSource.h>
#include <graphics/core/sources/ShaderSource.h>

#include <scripting/ScriptSystem.h>

namespace engine::core {

    class Application {

    public:
        Application() = default;
        virtual ~Application() = default;

    public:
        void run();
        void shutdown();

    public:
        static inline Application& get() {
            static Application instance;
            return instance;
        }

        inline const Scope<Window>& getWindow() {
            return _window;
        }

    public:
        // window events
        void onWindowClosed();
        void onWindowResized(const uint32_t &width, const uint32_t &height);
        // input keyboard events
        void onKeyPressed(event::KeyCode keyCode);
        void onKeyHold(event::KeyCode keyCode);
        void onKeyReleased(event::KeyCode keyCode);
        void onKeyTyped(event::KeyCode keyCode);
        // input mouse events
        void onMousePressed(event::KeyCode mouseCode);
        void onMouseRelease(event::KeyCode mouseCode);
        void onMouseScrolled(double xOffset, double yOffset);
        // input mouse cursor events
        void onCursorMoved(double xPos, double yPos);
        // input gamepad events
        void onGamepadConnected(s32 joystickId) const;
        void onGamepadDisconnected(s32 joystickId) const;

    protected:
        virtual void onCreate();
        virtual void onPrepare();
        virtual void onDestroy();
        virtual WindowProps createWindowProps();

    public:
        [[nodiscard]] float getAspectRatio() const;
        const uint32_t& getWindowWidth();
        const uint32_t& getWindowHeight();
        uint32_t getRefreshRate();
        void* getNativeWindow();
        void setWindowIcon(const std::string &filePath);
        Ref<FileDialog> createFileDialog();
        void setSampleSize(const uint32_t& samples);
        void setActiveScene(const Ref<Scene>& activeScene);
        void setActiveScene(const uint32_t& activeSceneId);

    protected:
        void pushFront(Layer* layer);
        void pushBack(Layer* overlay);

    private:
        void restart();
        void onUpdate();
        void updateRuntime(time::Time dt);

        void createGraphics();
        void createScripting();

    public:
        Scope<Input> input;
        std::vector<Ref<Scene>> scenes;
        Ref<Scene> activeScene;
        Ref<FrameBuffer> activeSceneFrame;
        Ref<FrameBuffer> screenFrame;
        time::FpsController fpsController;
        event::EventRegistry eventController;

    private:
        bool _isRunning = true;
        // core systems
        LayerStack _layerStack; // todo consider remove LayerStack. Instead create an Activity class.
        Scope<Window> _window;
        // graphics system
        Scope<RenderSystem> _renderSystem;
        // scripting system
        Scope<ScriptSystem> _scriptSystem;
    };

    Application* createApplication();

}