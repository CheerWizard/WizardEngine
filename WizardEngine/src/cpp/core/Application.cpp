//
// Created by mecha on 28.08.2021.
//

#include <core/Application.h>
#include <profiler/Profiler.h>
#include <time/Timer.h>

namespace engine::core {

    void Application::run() {
        onCreate();
        onPrepare();
        while (_isRunning) {
            onUpdate();
        }
        onDestroy();
    }

    void Application::onCreate() {
        PROFILE_FUNCTION();
        ENGINE_INFO("onCreate()");

        io::TextureFile::setFlipTexture(false);

        if (!ProjectProps::createFromFile("properties.yaml", projectProps)) {
            ENGINE_WARN("Application: Unable to create properties from properties.yaml");
        }

        _window = createScope<Window>(projectProps.windowProps);
        if (projectProps.icon != "") {
            setWindowIcon(projectProps.icon);
        }

        createGraphics();
        Input::create(_window->getNativeWindow());

#ifdef VISUAL
        Visual::init(getNativeWindow());
        Visual::fullScreen = projectProps.windowProps.fullscreen;
        Visual::openDockspace = projectProps.windowProps.dockspace;
#endif
        // init audio
        audio::DeviceManager::createContext();
        // init default post effect renderers
        initHDR();
        initBlur();
        initSharpen();
        initEdgeDetection();
        initGaussianBlur();
        initTextureMixer();

        if (projectProps.launcher != "") {
            setActiveScene(io::LocalAssetManager::loadScene(projectProps.launcher.c_str()));
        } else {
            ENGINE_WARN("Application: Launcher scene not specified in properties.yaml!");
        }

        RenderSystem::hdrEnvRenderer.init();
    }

    void Application::onPrepare() {
        PROFILE_FUNCTION();
        graphics::enableSRGB();
        _window->onPrepare();
        _window->setInCenter();
        setSampleSize(projectProps.windowProps.sampleSize);
        _layerStack.onPrepare();
        loadGamepadMappings("assets/mappings/game_controller.txt");
        ENGINE_INFO("screenRenderer.onWindowResized");
        RenderSystem::screenRenderer.onWindowResized(getWindowWidth(), getWindowHeight());
    }

    void Application::setSkybox(Ref<Scene> &scene, const Entity& skybox) const {
        scene->setSkybox(skybox);
        RenderSystem::skyboxRenderer.upload(scene->getSkybox().get<Skybox>());
    }

    void Application::setSkyCube(Ref<Scene> &scene, const char* skyboxName, u32 skyboxId) const {
        setSkybox(scene, SkyboxCube(
                skyboxName,
                scene.get(),
                CubeMapTextureComponent(skyboxId, TextureType::CUBE_MAP)
        ));
    }

    void Application::setSkyCube(Ref<Scene>& scene, const char* skyboxName, const vector<TextureFace>& skyboxFaces) const {
        u32 skyboxId = TextureBuffer::load(skyboxFaces);
        TextureBuffer::setDefaultParamsCubeMap(skyboxId);
        setSkyCube(scene, skyboxName, skyboxId);
    }

    void Application::setHdrEnv(Ref<Scene>& scene, const Entity& hdrEnv) const {
        scene->setHdrEnv(hdrEnv);
        RenderSystem::hdrEnvRenderer.upload(scene->getHdrEnv().get<HdrEnv>());
    }

    void Application::setHdrEnvCube(Ref<Scene>& scene, const char* filepath) const {
        setHdrEnv(
                scene,
                HdrEnvCube(
                        "HdrEnvCube",
                        scene.get(),
                        TextureComponent(
                                TextureBuffer::load(filepath, io::Spectrum::HDR),
                                TextureType::TEXTURE_2D,
                                IntUniform { "hdrEnv", 0 }
                        )
                )
        );
    }

    void Application::onDestroy() {
        PROFILE_FUNCTION();
        ENGINE_INFO("onDestroy()");
        RenderSystem::onDestroy();
        ScriptSystem::onDestroy();
        audio::MediaPlayer::clear();
        audio::DeviceManager::clear();
#ifdef VISUAL
        Visual::release();
#endif
    }

    void Application::onUpdate() {
        PROFILE_FUNCTION();
        // measure dt time
        Timer timer("Application::onUpdate()", 30);
        // update runtime system
        onRuntimeUpdate(dt);
        // update tools system
#ifdef VISUAL
        Visual::begin();
        _layerStack.onVisualDraw(dt);
        Visual::end();
#endif
        // poll events + swap chain
        if (enableMouseCursor) {
            Input::updateMousePosition();
        }
        onEventUpdate();
        _window->onUpdate();

        dt = timer.stop();

        PROFILE_ON_FRAME_UPDATED();
    }

    void Application::pushFront(Layer *layer) {
        ENGINE_INFO("Pushing layer : {0}", layer->getTag());
        _layerStack.pushFront(layer);
    }

    void Application::pushBack(Layer *layer) {
        ENGINE_INFO("Pushing overlay : {0}", layer->getTag());
        _layerStack.pushBack(layer);
    }

    void Application::onWindowClosed() {
        ENGINE_INFO("Application : onWindowClosed()");
        _layerStack.onWindowClosed();
        EventRegistry::onWindowClosed.function();
#ifdef VISUAL
        Visual::onWindowClosed();
#endif
        shutdown();
    }

    void Application::onWindowResized(u32 width, u32 height) {
        PROFILE_FUNCTION();
        ENGINE_INFO("Application : onWindowResized({0}, {1})", width, height);
        if (width == 0 || height == 0) return;

        _layerStack.onWindowResized(width, height);
//        activeSceneFrame->resize(width, height);
//        screenFrame->resize(width, height);
        EventRegistry::onWindowResized.function(width, height);
        RenderSystem::screenRenderer.onWindowResized(width, height);
#ifdef VISUAL
        Visual::onWindowResized(width, height);
#endif
    }

    void Application::onKeyPressed(event::KeyCode keyCode) {
        PROFILE_FUNCTION();
        _layerStack.onKeyPressed(keyCode);
        EventRegistry::onKeyPressedMap[keyCode].function(keyCode);
#ifdef VISUAL
        Visual::onKeyPressed(keyCode);
#endif
    }

    void Application::onKeyHold(event::KeyCode keyCode) {
        PROFILE_FUNCTION();
        _layerStack.onKeyHold(keyCode);
        EventRegistry::onKeyHoldMap[keyCode] = true;
#ifdef VISUAL
        Visual::onKeyHold(keyCode);
#endif
    }

    void Application::onKeyReleased(event::KeyCode keyCode) {
        PROFILE_FUNCTION();
        _layerStack.onKeyReleased(keyCode);
        EventRegistry::onKeyReleasedMap[keyCode].function(keyCode);
        EventRegistry::onKeyHoldMap[keyCode] = false;
#ifdef VISUAL
        Visual::onKeyReleased(keyCode);
#endif
    }

    void Application::onMousePressed(event::MouseCode mouseCode) {
        PROFILE_FUNCTION();
        _layerStack.onMousePressed(mouseCode);
        EventRegistry::onMousePressedMap[mouseCode].function(mouseCode);
        EventRegistry::mouseHoldMap[mouseCode] = true;
#ifdef VISUAL
        Visual::onMousePressed(mouseCode);
#endif
    }

    void Application::onMouseRelease(event::MouseCode mouseCode) {
        PROFILE_FUNCTION();
        _layerStack.onMouseRelease(mouseCode);
        EventRegistry::onMouseReleasedMap[mouseCode].function(mouseCode);
        EventRegistry::mouseHoldMap[mouseCode] = false;
#ifdef VISUAL
        Visual::onMouseRelease(mouseCode);
#endif
    }

    void Application::onMouseScrolled(double xOffset, double yOffset) {
        PROFILE_FUNCTION();
        _layerStack.onMouseScrolled(xOffset, yOffset);
        EventRegistry::onMouseScrolled.function(xOffset, yOffset);
#ifdef VISUAL
        Visual::onMouseScrolled(xOffset, yOffset);
#endif
    }

    void Application::onCursorMoved(double xPos, double yPos) {
        PROFILE_FUNCTION();
        _layerStack.onCursorMoved(xPos, yPos);
        EventRegistry::onCursorMoved.function(xPos, yPos);
#ifdef VISUAL
        Visual::onCursorMoved(xPos, yPos);
#endif
    }

    void Application::onKeyTyped(event::KeyCode keyCode) {
        PROFILE_FUNCTION();
        _layerStack.onKeyTyped(keyCode);
#ifdef VISUAL
        Visual::onKeyTyped(keyCode);
#endif
    }

    float Application::getAspectRatio() const {
        return _window->getAspectRatio();
    }

    void Application::setActiveScene(const Ref<Scene>& scene) {
        PROFILE_FUNCTION();
        activeScene = scene;
        RenderSystem::activeScene = activeScene;
        ScriptSystem::activeScene = activeScene;
        Physics::activeScene = activeScene;
        RenderSystem::onUpdate();
    }

    void Application::setActiveScene(u32 sceneIndex) {
        PROFILE_FUNCTION();
        setActiveScene(scenes.at(sceneIndex));
    }

    void Application::restart() {
        onDestroy();
        onCreate();
    }

    int Application::getWindowWidth() {
        return _window->getWidth();
    }

    int Application::getWindowHeight() {
        return _window->getHeight();
    }

    int Application::getRefreshRate() {
        return _window->getRefreshRate();
    }

    void *Application::getNativeWindow() {
        return _window->getNativeWindow();
    }

    void Application::setWindowIcon(const std::string &filePath) {
        _window->setWindowIcon(filePath);
    }

    Ref<tools::FileDialog> Application::createFileDialog() {
        return createRef<tools::FileDialog>(_window->getNativeWindow());
    }

    void Application::setSampleSize(int samples) {
        _window->setSampleSize(samples);
        // update active scene frame format
        FrameBufferFormat activeSceneFrameFormat;
        activeSceneFrameFormat.colorAttachments = {
                { ColorFormat::RGBA8, ColorFormat::RGBA },
                { ColorFormat::RGBA8, ColorFormat::RGBA },
                { ColorFormat::RED_I32, ColorFormat::RED_INTEGER }
        };
        activeSceneFrameFormat.renderBufferAttachment = { DepthStencilFormat::DEPTH24STENCIL8 };
        activeSceneFrameFormat.width = _window->getWidth();
        activeSceneFrameFormat.height = _window->getHeight();
        activeSceneFrameFormat.samples = 1;
        activeSceneFrame->updateFormat(activeSceneFrameFormat);
        // update msaa frame format
        FrameBufferFormat msaaFormat(activeSceneFrameFormat);
        msaaFormat.samples = samples;
        msaaFrame->updateFormat(msaaFormat);
        // update shadows frame format
        FrameBufferFormat shadowsFormat;
        shadowsFormat.depthAttachment = { DepthFormat::DEPTH, DepthFormat::U_DEPTH };
        shadowsFormat.width = 1024;
        shadowsFormat.height = 1024;
        shadowsFrame->updateFormat(shadowsFormat);
        // resolve size issue
        onWindowResized(_window->getWidth(), _window->getHeight());
    }

    void Application::createGraphics() {
        graphics::initContext(_window->getNativeWindow());
        setClearColor(0, 0, 0, 1);
        activeSceneFrame = createRef<FrameBuffer>();
        msaaFrame = createRef<FrameBuffer>();
        shadowsFrame = createRef<FrameBuffer>();
        RenderSystem::sceneFrame = activeSceneFrame;
        RenderSystem::msaaFrame = msaaFrame;
        RenderSystem::shadowsFrame = shadowsFrame;
        RenderSystem::setRenderSystemCallback(this);
        RenderSystem::screenRenderer.init();
        RenderSystem::skyboxRenderer.init();
    }

    void Application::onRuntimeUpdate(Time dt) {
        if (!activeScene->isEmpty()) {
            Physics::onUpdate(dt);
            ScriptSystem::onUpdate(dt);
            RenderSystem::onUpdate();
            _layerStack.onUpdate(dt);
        } else {
            ENGINE_WARN("Active scene is empty!");
        }
    }

    void Application::shutdown() {
        _isRunning = false;
    }

    void Application::onGamepadConnected(s32 joystickId) {
        ENGINE_INFO("onGamepadConnected(id: {0})", joystickId);
        Input::setJoystickId(joystickId);
        isJoystickConnected = true;
    }

    void Application::onGamepadDisconnected(s32 joystickId) {
        ENGINE_INFO("onGamepadDisconnected(id: {0})", joystickId);
        Input::setJoystickId(joystickId);
        isJoystickConnected = false;
    }

    Application* Application::instance = nullptr;

    Application::Application() {
        instance = this;
    }

    Application::~Application() {
        instance = nullptr;
    }

    void Application::loadGamepadMappings(const char *mappingsFilePath) {
        _window->loadGamepadMappings(mappingsFilePath);
    }

    void Application::onEventUpdate() {
        if (isJoystickConnected) {
            ENGINE_INFO("Joystick CONNECTED! Polling buttons and axes...");
            const auto& gamepadState = Input::getGamepadState();
            const auto& buttons = gamepadState.buttons;
            const auto& axis = gamepadState.axes;

            for (auto& gamepadButtonPressed : EventRegistry::onGamepadButtonPressedMap) {
                if (buttons[gamepadButtonPressed.first] == event::PRESS) {
                    gamepadButtonPressed.second.function(gamepadButtonPressed.first);
                }
            }

            for (auto& gamepadButtonReleased : EventRegistry::onGamepadButtonReleasedMap) {
                if (buttons[gamepadButtonReleased.first] == event::RELEASE) {
                    gamepadButtonReleased.second.function(gamepadButtonReleased.first);
                }
            }

            auto gamepadRollLeft = event::GamepadRoll {
                    axis[event::PAD_LEFT_ROLL.x],
                    axis[event::PAD_LEFT_ROLL.y],
                    axis[event::PAD_LEFT_ROLL.trigger] != -1,
            };
            if (gamepadRollLeft != EventRegistry::inactiveGamepadRollLeft) {
                EventRegistry::onGamepadRollLeft.function(gamepadRollLeft);
            }

            auto gamepadRollRight = event::GamepadRoll {
                    axis[event::PAD_RIGHT_ROLL.x],
                    axis[event::PAD_RIGHT_ROLL.y],
                    axis[event::PAD_RIGHT_ROLL.trigger] != -1,
            };
            if (gamepadRollRight != EventRegistry::inactiveGamepadRollRight) {
                EventRegistry::onGamepadRollRight.function(gamepadRollRight);
            }
        } else {
            ENGINE_WARN("Joystick DISCONNECTED! Polling joystick state...");
            isJoystickConnected = Input::isJoystickConnected();
        }
    }

    void Application::onFrameBegin(const Ref<FrameBuffer> &frameBuffer) {
        if (!enableMouseHovering) return;

        ENGINE_INFO("Application::onFrameBegin()");
//        frameBuffer->removeAttachment(frameBuffer->getColorAttachmentsSize() - 1, -1);
    }

    void Application::onFrameEnd(const Ref<FrameBuffer> &frameBuffer) {
        if (!enableMouseHovering) return;

        ENGINE_INFO("Application::onFrameEnd()");

        auto[mx, my] = ImGui::GetMousePos();
        auto& viewportBound = activeScene->viewportBounds;
        mx -= viewportBound[0].x;
        my -= viewportBound[0].y;
        glm::vec2 viewportSize = viewportBound[1] - viewportBound[0];
        my = viewportSize.y - my;
        int mouseX = (int)mx;
        int mouseY = (int)my;

        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
            int uuid = frameBuffer->readPixel(
                    frameBuffer->getColorAttachmentsSize() - 1,
                    mouseX, mouseY
            );
            hoveredEntity = activeScene->findEntity(uuid);
        }
    }

    void Application::initHDR() {
        FrameBufferFormat hdrFrameFormat;
        hdrFrameFormat.colorAttachments = {
                { ColorFormat::RGBA16F, ColorFormat::RGBA, PixelsType::FLOAT },
        };
        hdrFrameFormat.width = _window->getWidth();
        hdrFrameFormat.height = _window->getHeight();
        RenderSystem::hdrEffectRenderer = { hdrFrameFormat };
        hdrEffect = RenderSystem::hdrEffectRenderer.getHdrEffect();
    }

    void Application::initBlur() {
        FrameBufferFormat blurFrameFormat;
        blurFrameFormat.colorAttachments = {
                { ColorFormat::RGBA8, ColorFormat::RGBA },
        };
        blurFrameFormat.width = _window->getWidth();
        blurFrameFormat.height = _window->getHeight();
        BlurEffectRenderer blurEffectRenderer(blurFrameFormat);
        blurEffect = blurEffectRenderer.getBlurEffect();
        RenderSystem::postEffectRenderers.emplace_back(blurEffectRenderer);
    }

    void Application::initSharpen() {
        FrameBufferFormat sharpenFrameFormat;
        sharpenFrameFormat.colorAttachments = {
                { ColorFormat::RGBA8, ColorFormat::RGBA },
        };
        sharpenFrameFormat.width = _window->getWidth();
        sharpenFrameFormat.height = _window->getHeight();
        SharpenEffectRenderer sharpenEffectRenderer(sharpenFrameFormat);
        sharpenEffect = sharpenEffectRenderer.getSharpenEffect();
        RenderSystem::postEffectRenderers.emplace_back(sharpenEffectRenderer);
    }

    void Application::initEdgeDetection() {
        FrameBufferFormat edgeDetectionFrameFormat;
        edgeDetectionFrameFormat.colorAttachments = {
                { ColorFormat::RGBA8, ColorFormat::RGBA },
        };
        edgeDetectionFrameFormat.width = _window->getWidth();
        edgeDetectionFrameFormat.height = _window->getHeight();
        EdgeDetectionEffectRenderer edgeDetectionEffectRenderer(edgeDetectionFrameFormat);
        edgeDetectionEffect = edgeDetectionEffectRenderer.getEdgeDetectionEffect();
        RenderSystem::postEffectRenderers.emplace_back(edgeDetectionEffectRenderer);
    }

    void Application::initGaussianBlur() {
        FrameBufferFormat gaussianBlurFrameFormat;
        gaussianBlurFrameFormat.colorAttachments = {
                { ColorFormat::RGBA8, ColorFormat::RGBA },
        };
        gaussianBlurFrameFormat.width = _window->getWidth();
        gaussianBlurFrameFormat.height = _window->getHeight();
        GaussianBlurEffectRenderer gaussianBlurEffectRenderer(gaussianBlurFrameFormat);
        gaussianBlurEffect = gaussianBlurEffectRenderer.getGaussianBlurEffect();
        RenderSystem::gaussianBlurRenderer = gaussianBlurEffectRenderer;
    }

    void Application::initTextureMixer() {
        FrameBufferFormat textureMixerFrameFormat;
        textureMixerFrameFormat.colorAttachments = {
                { ColorFormat::RGBA8, ColorFormat::RGBA },
        };
        textureMixerFrameFormat.width = _window->getWidth();
        textureMixerFrameFormat.height = _window->getHeight();
        RenderSystem::textureMixer = { textureMixerFrameFormat };
    }

    void Application::onVisualDraw(time::Time dt) {
        // this method will be called only from derived class that defines VISUAL macro
    }

    void Application::pushScenes(const vector<Ref<Scene>> &scenes) {
        if (scenes.empty()) return;
        this->scenes.clear();
        this->scenes = scenes;
        setActiveScene(0);
    }
}

namespace engine::event {

    Action<> EventRegistry::onWindowClosed;
    Action<const uint32_t&, const uint32_t&> EventRegistry::onWindowResized;

    EventRegistry::KeyCodeMap EventRegistry::onKeyPressedMap;
    EventRegistry::KeyHoldMap EventRegistry::onKeyHoldMap;
    EventRegistry::KeyCodeMap EventRegistry::onKeyReleasedMap;
    EventRegistry::KeyCodeMap EventRegistry::onKeyTypedMap;

    EventRegistry::MouseCodeMap EventRegistry::onMousePressedMap;
    EventRegistry::MouseCodeMap EventRegistry::onMouseReleasedMap;

    EventRegistry::MouseHoldMap EventRegistry::mouseHoldMap;

    Action<double, double> EventRegistry::onMouseScrolled;
    Action<double, double> EventRegistry::onCursorMoved;

    EventRegistry::GamepadButtonMap EventRegistry::onGamepadButtonPressedMap;
    EventRegistry::GamepadButtonMap EventRegistry::onGamepadButtonReleasedMap;

    Action<GamepadRoll> EventRegistry::onGamepadRollLeft;
    Action<GamepadRoll> EventRegistry::onGamepadRollRight;
    GamepadRoll EventRegistry::inactiveGamepadRollLeft;
    GamepadRoll EventRegistry::inactiveGamepadRollRight;

    bool EventRegistry::keyHold(KeyCode keyCode) {
        return onKeyHoldMap[keyCode];
    }

    bool EventRegistry::mouseHold(MouseCode mouseCode) {
        return mouseHoldMap[mouseCode];
    }
}