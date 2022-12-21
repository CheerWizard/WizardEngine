//
// Created by mecha on 28.08.2021.
//

#pragma once

#include <core/core.h>
#include <core/LayerStack.h>
#include <core/String.h>
#include <core/Memory.h>
#include <core/ProjectManager.h>
#include <core/job_system.h>

#include <time/Time.h>

#include <math/RayCast.h>

#include <event/Events.h>
#include <event/GamepadCodes.h>

#include <platform/core/Window.h>
#include <platform/core/Input.h>
#include <platform/tools/FileDialog.h>
#include <platform/graphics/graphics_context.h>
#include <platform/graphics/tools/VideoStats.h>

#include <graphics/core/RenderSystem.h>
#include <graphics/core/sources/ShaderSource.h>
#include <graphics/core/io/ModelFile.h>
#include <graphics/core/geometry/Point.h>
#include <graphics/camera/Camera.h>
#include <graphics/skybox/Skybox.h>
#include <graphics/core/geometry/Quad.h>
#include <graphics/GraphicsObject.h>
#include <graphics/materials/Color.h>
#include <graphics/materials/Material.h>
#include <graphics/hdr_env/hdr_env.h>
#include <graphics/camera/CameraShaderScript.h>

#include <scripting/ScriptSystem.h>
#include <scripting/ScriptManager.h>

#include <audio/audio_device_manager.h>
#include <audio/audio_player.h>
#include <audio/audio_source.h>

#include <network/network_server.h>
#include <network/network_client.h>

#include <serialization/AssetManager.h>

#include <physics/Physics.h>

#include <thread/Thread.h>

using namespace engine::core;
using namespace engine::graphics;
using namespace engine::event;
using namespace engine::time;
using namespace engine::network;
using namespace engine::ecs;
using namespace engine::physics;
using namespace engine::scripting;
using namespace engine::thread;

#define KEY_PRESSED(key) engine::event::EventRegistry::onKeyPressedMap[key] =
#define KEY_RELEASED(key) engine::event::EventRegistry::onKeyReleasedMap[key] =
#define KEY_TYPED(key) engine::event::EventRegistry::onKeyTypedMap[key] =
#define KEY_HOLD(key) engine::event::EventRegistry::onKeyHoldMap[key] =

#define GAMEPAD_PRESSED(btn) engine::event::EventRegistry::onGamepadButtonPressedMap[btn] =
#define GAMEPAD_RELEASED(btn) engine::event::EventRegistry::onGamepadButtonReleasedMap[btn] =
#define GAMEPAD_ROLL_LEFT(action) engine::event::EventRegistry::onGamepadRollLeft.function =
#define GAMEPAD_ROLL_RIGHT(action) engine::event::EventRegistry::onGamepadRollRight.function =

#ifdef VISUAL
#include <visual/Visual.h>
#include <visual/Console.h>
#include <visual/Widgets.h>
#include <visual/Troubleshoot.h>
#include <visual/ProjectsPanel.h>
#include <visual/AssetBrowser.h>
#include <visual/Log.h>
#include <visual/ImageLayout.h>
#include <visual/MaterialPanel.h>
#include <visual/LightPanel.h>
#include <visual/SceneHierarchy.h>
#include <visual/Toolbar.h>

using namespace engine::visual;
#endif

namespace engine::core {

    // The entry point in core hierarchy, behaves as singleton.
    // It's standalone class, which can NOT be created several times and should be created only once!
    class ENGINE_API Application : RenderSystemCallback {

    public:
        Application();
        virtual ~Application();

    public:
        void run();

    public:
        static inline Application& get() {
            return *instance;
        }

        inline const Scope<Window>& getWindow() {
            return _window;
        }

        inline unordered_map<uuid, Ref<Scene>>& getScenes() {
            return scenes;
        }

    public:
        // window events
        void onWindowClosed();
        void onWindowResized(u32 width, u32 height);
        // input keyboard events
        void onKeyPressed(event::KeyCode keyCode);
        void onKeyHold(event::KeyCode keyCode);
        void onKeyReleased(event::KeyCode keyCode);
        void onKeyTyped(event::KeyCode keyCode);
        // input mouse events
        void onMousePressed(event::MouseCode mouseCode);
        void onMouseRelease(event::MouseCode mouseCode);
        void onMouseScrolled(double xOffset, double yOffset);
        // input mouse cursor events
        void onCursorMoved(double xPos, double yPos);
        // input gamepad events
        void onGamepadConnected(s32 joystickId);
        void onGamepadDisconnected(s32 joystickId);
        // render system callbacks
        void onFrameBegin(const Ref<FrameBuffer> &frameBuffer) override;
        void onFrameEnd(const Ref<FrameBuffer> &frameBuffer) override;

    protected:
        virtual void onCreate();
        virtual void onDestroy();
        virtual void onVisualDraw(time::Time dt);

    public:
        [[nodiscard]] float getAspectRatio() const;
        int getWindowWidth();
        int getWindowHeight();
        int getRefreshRate();
        void* getNativeWindow();
        void setWindowIcon(const std::string &filePath);
        Ref<tools::FileDialog> createFileDialog();
        void setSampleSize(int samples);

        void setActiveScene(const Ref<Scene>& activeScene);
        void addScene(const Ref<Scene>& scene);
        void addScenes(const vector<Ref<Scene>>& scenes);
        void removeScene(const uuid& sceneId);
        void clearScenes();

        void loadGamepadMappings(const char* mappingsFilePath);

        Ref<Scene> newScene(const std::string& sceneName);
        Ref<Renderer> createBatchRenderer();
        Ref<Renderer> createInstanceRenderer();

    protected:
        void pushFront(Layer* layer);
        void pushBack(Layer* overlay);

    private:
        void shutdown();
        void restart();

        void onUpdate();
        void onSimulationUpdate(time::Time dt);
        void onEventUpdate();

        void createGraphics();
        // init post effect renderers
        void initHDR();
        void initBlur();
        void initSharpen();
        void initEdgeDetection();
        void initGaussianBlur();
        void initTextureMixer();

        void bindCamera(Camera3D& camera);

        void onPadA();
        void onPadB();
        void onPadX();
        void onPadY();
        void onGamepadRollLeft(const GamepadRoll& roll);
        void onGamepadRollRight(const GamepadRoll& roll);

        vector<Batch3d> loadModel(const uuid& sceneId);
        vector<Batch3d> loadModel(const Ref<Scene>& scene);

    public:
        Ref<Scene> activeScene = nullptr;
        Ref<FrameBuffer> activeSceneFrame;
        Ref<FrameBuffer> msaaFrame;
        Ref<FrameBuffer> shadowsFrame;
        time::Time dt = 6;
        bool isJoystickConnected = false;
        // hover entity with mouse cursor
        bool enableMouseHovering = false;
        Entity hoveredEntity;
        Entity selectedEntity;
        // mouse cursor tracker
        bool enableMouseCursor = false;
        // post effects
        Ref<HdrEffect> hdrEffect;
        Ref<BlurEffect> blurEffect;
        Ref<SharpenEffect> sharpenEffect;
        Ref<EdgeDetectionEffect> edgeDetectionEffect;
        Ref<GaussianBlurEffect> gaussianBlurEffect;
        ProjectProps projectProps;

    private:
        static Application* instance;
        bool _isRunning = true;
        // core systems
        LayerStack _layerStack;
        Scope<Window> _window;
        unordered_map<uuid, Ref<Scene>> scenes;
    };

    Application* createApplication();

}