//
// Created by mecha on 28.08.2021.
//

#include <core/Application.h>
#include <profiler/Profiler.h>
#include <time/Timer.h>

#include <future>

namespace engine::core {

    void Application::run() {
        onCreate();
        while (_isRunning) {
            onUpdate();
        }
        onDestroy();
    }

    void Application::onCreate() {
        PROFILE_FUNCTION();
        ENGINE_INFO("onCreate()");
        // setup job system
        auto& jobSystem = JobSystem<>::get();
        // setup window, input, graphics
        jobSystem.renderScheduler->execute([this]() {
            io::TextureFile::setFlipTexture(false);
            if (!ProjectProps::createFromFile("properties.yaml", projectProps)) {
                ENGINE_WARN("Application: Unable to create properties from properties.yaml");
            }
            _window = createScope<Window>(projectProps.windowProps);
            loadGamepadMappings("assets/mappings/game_controller.txt");
            if (projectProps.icon != "") {
                setWindowIcon(projectProps.icon);
            }

            Input::create(_window->getNativeWindow());
            createGraphics();
            initHDR();
            initBlur();
            initSharpen();
            initEdgeDetection();
            initGaussianBlur();
            initTextureMixer();
            // setup common scene renderers
            createBatchRenderer();
            createInstanceRenderer();
            RenderSystem::hdrEnvRenderer.init();
            // setup ImGui
#ifdef VISUAL
            Visual::init(getNativeWindow());
            Visual::fullScreen = projectProps.windowProps.fullscreen;
            Visual::openDockspace = projectProps.windowProps.dockspace;
            AssetBrowser::create(getNativeWindow());
            MaterialPanel::create(getNativeWindow());
#endif
            _window->onPrepare();
            _window->setInCenter();
            graphics::enableSRGB();
            setSampleSize(projectProps.windowProps.sampleSize);
        });
        // setup audio
        jobSystem.audioScheduler->execute([]() {
            audio::DeviceManager::createContext();
        });
        // setup network
        jobSystem.networkScheduler->execute([]() {
        });
        // setup something heavy
        jobSystem.threadPoolScheduler->execute([]() {
        });
        // at least do some job on main thread
        if (projectProps.launcher != "") {
            setActiveScene(io::LocalAssetManager::loadScene(projectProps.launcher.c_str()));
        } else {
            ENGINE_WARN("Application: Launcher scene not specified in properties.yaml!");
        }
        // we must sync with other jobs to continue further.
        // otherwise, we will cause race conditions and bugs in onPrepare()
        jobSystem.waitAll();
        _layerStack.onPrepare();
    }

    void Application::onDestroy() {
        PROFILE_FUNCTION();
        ENGINE_INFO("onDestroy()");
        JobSystem<>::get().renderScheduler->execute([]() {
            RenderSystem::onDestroy();
#ifdef VISUAL
            Visual::release();
#endif
        });
        JobSystem<>::get().audioScheduler->execute([]() {
            audio::MediaPlayer::clear();
            audio::DeviceManager::clear();
        });
        ScriptSystem::onDestroy();
        JobSystem<>::get().renderScheduler->wait();
        JobSystem<>::get().audioScheduler->wait();
    }

    void Application::onUpdate() {
        PROFILE_FUNCTION();
        // measure dt time
        Timer timer("Application::onUpdate()", 30);

        auto& jobSystem = JobSystem<>::get();

        // update graphics, window, input, tools
        jobSystem.renderScheduler->execute([this]() {
            if (activeScene && !activeScene->isEmpty()) {
                RenderSystem::onUpdate();
            }
#ifdef VISUAL
            Visual::begin();
            Visual::onUpdate(dt);
            _layerStack.onVisualDraw(dt);
            Visual::end();
#endif
            // poll events + swap chain
            if (enableMouseCursor) {
                Input::updateMousePosition();
            }
            onEventUpdate();
            _window->onUpdate();
        });
        // update simulation systems
        onSimulationUpdate(dt);
        jobSystem.renderScheduler->wait();

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
        if (activeScene) {
            activeScene->getCamera().setAspectRatio(width, height);
        }
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
#ifdef VISUAL
        Visual::onKeyPressed(keyCode);
        if (Visual::blocksKeyboard())
            return;
#endif
        _layerStack.onKeyPressed(keyCode);
        EventRegistry::onKeyPressedMap[keyCode].function(keyCode);
    }

    void Application::onKeyHold(event::KeyCode keyCode) {
        PROFILE_FUNCTION();
#ifdef VISUAL
        Visual::onKeyHold(keyCode);
        if (Visual::blocksKeyboard())
            return;
#endif
        _layerStack.onKeyHold(keyCode);
        if (activeScene) {
            auto& camera = activeScene->getCamera();
            switch (keyCode) {
                case W:
                    camera.applyMove(UP);
                    break;
                case A:
                    camera.applyMove(LEFT);
                    break;
                case S:
                    camera.applyMove(DOWN);
                    break;
                case D:
                    camera.applyMove(RIGHT);
                    break;
                case Q:
                    camera.applyRotate(LEFT_Z);
                    break;
                case E:
                    camera.applyRotate(RIGHT_Z);
                    break;
                case Z:
                    camera.applyZoom(ZOOM_IN);
                    break;
                case X:
                    camera.applyZoom(ZOOM_OUT);
                    break;
            }
        }
        EventRegistry::onKeyHoldMap[keyCode] = true;
    }

    void Application::onKeyReleased(event::KeyCode keyCode) {
        PROFILE_FUNCTION();
#ifdef VISUAL
        Visual::onKeyReleased(keyCode);
        if (Visual::blocksKeyboard())
            return;
#endif
        _layerStack.onKeyReleased(keyCode);
        EventRegistry::onKeyReleasedMap[keyCode].function(keyCode);
        EventRegistry::onKeyHoldMap[keyCode] = false;
    }

    void Application::onMousePressed(event::MouseCode mouseCode) {
        PROFILE_FUNCTION();
#ifdef VISUAL
        Visual::onMousePressed(mouseCode);
        if (Visual::blocksMouse())
            return;
#endif
        _layerStack.onMousePressed(mouseCode);
        EventRegistry::onMousePressedMap[mouseCode].function(mouseCode);
        EventRegistry::mouseHoldMap[mouseCode] = true;
    }

    void Application::onMouseRelease(event::MouseCode mouseCode) {
        PROFILE_FUNCTION();
#ifdef VISUAL
        Visual::onMouseRelease(mouseCode);
        if (Visual::blocksMouse())
            return;
#endif
        _layerStack.onMouseRelease(mouseCode);
        EventRegistry::onMouseReleasedMap[mouseCode].function(mouseCode);
        EventRegistry::mouseHoldMap[mouseCode] = false;
    }

    void Application::onMouseScrolled(double xOffset, double yOffset) {
        PROFILE_FUNCTION();
#ifdef VISUAL
        Visual::onMouseScrolled(xOffset, yOffset);
        if (Visual::blocksMouse())
            return;
#endif
        _layerStack.onMouseScrolled(xOffset, yOffset);
        EventRegistry::onMouseScrolled.function(xOffset, yOffset);
    }

    void Application::onCursorMoved(double xPos, double yPos) {
        PROFILE_FUNCTION();
#ifdef VISUAL
        Visual::onCursorMoved(xPos, yPos);
        if (Visual::blocksMousePos())
            return;
#endif
        _layerStack.onCursorMoved(xPos, yPos);
        EventRegistry::onCursorMoved.function(xPos, yPos);
    }

    void Application::onKeyTyped(event::KeyCode keyCode) {
        PROFILE_FUNCTION();
#ifdef VISUAL
        Visual::onKeyTyped(keyCode);
        if (Visual::blocksTextInput())
            return;
#endif
        _layerStack.onKeyTyped(keyCode);
    }

    float Application::getAspectRatio() const {
        return _window->getAspectRatio();
    }

    void Application::setActiveScene(const Ref<Scene>& scene) {
        PROFILE_FUNCTION();
        scenes[scene->getId()] = scene;
        activeScene = scene;
        selectedEntity = Entity(activeScene.get());
        hoveredEntity = Entity(activeScene.get());
        bindCamera(activeScene->getCamera());
        RenderSystem::activeScene = activeScene;
        ScriptSystem::activeScene = activeScene;
        Physics::activeScene = activeScene;
        RenderSystem::onUpdate();
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

    void Application::onSimulationUpdate(Time dt) {
        if (activeScene && !activeScene->isEmpty()) {
            Physics::onUpdate(dt);
            ScriptSystem::onUpdate(dt);
            _layerStack.onUpdate(dt);

            auto& camera = activeScene->getCamera();
            camera.onUpdate(dt);

            if (EventRegistry::mouseHold(ButtonLeft)) {
                RUNTIME_INFO("mouseHold: button left");
                camera.applyMouseRotate();
            }

            if (EventRegistry::mouseHold(ButtonRight)) {
                RUNTIME_INFO("mouseHold: button right");
                camera.applyMouseMove();
            }

            if (EventRegistry::mouseHold(ButtonMiddle)) {
                RUNTIME_INFO("mouseHold: button middle");
                camera.applyMouseZoom();
            }
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
            activeScene->findEntity(uuid, hoveredEntity);
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

    void Application::addScene(const Ref<Scene>& scene) {
        scenes[scene->getId()] = scene;
    }

    void Application::removeScene(const uuid& sceneId) {
        scenes.erase(sceneId);
    }

    void Application::clearScenes() {
        scenes.clear();
    }

    void Application::addScenes(const vector<Ref<Scene>> &newScenes) {
        for (const auto& newScene : newScenes) {
            scenes[newScene->getId()] = newScene;
        }
    }

    vector<Batch3d> Application::loadModel(const Ref<Scene>& scene) {
        // we need to flip textures as they will be loaded vice versa
        io::TextureFile::setFlipTexture(true);
        io::Model model = io::ModelFile<BatchVertex<Vertex3d>>::read(
                "assets/SamuraiHelmet/source/SamuraiHelmet.fbx.fbx",
                "assets/SamuraiHelmet/textures"
        );
        RUNTIME_INFO("ModelFile read: onSuccess");

        vector<Batch3d> entities;
        for (int i = 0; i < model.meshes.size(); i++) {
            auto modelMesh = model.meshes[i];
            BaseMeshComponent<BatchVertex<Vertex3d>> meshComponent;
            meshComponent.mesh = modelMesh.toMesh<BatchVertex<Vertex3d>>([](const io::ModelVertex& modelVertex) {
                return BatchVertex<Vertex3d> {
                        modelVertex.position,
                        modelVertex.uv,
                        modelVertex.normal,
                        modelVertex.tangent,
                        modelVertex.bitangent,
                        0
                };
            });
            std::stringstream ss;
            ss << "Entity_" << i;
            auto newEntity = Batch3d(scene.get());
            newEntity.get<TagComponent>()->tag = ss.str();
            newEntity.getTransform().position = { 1, 1, 1 };
            newEntity.applyTransform();
            newEntity.add<BaseMeshComponent<BatchVertex<Vertex3d>>>(meshComponent);
            newEntity.add<Material>(modelMesh.material);
            entities.emplace_back(newEntity);
        }

        return entities;
    }

    vector<Batch3d> Application::loadModel(const uuid& sceneId) {
        return loadModel(scenes.at(sceneId));
    }

    Ref<Scene> Application::newScene(const std::string& sceneName) {
        // setup scene, entities, components
        auto scene = createRef<Scene>(sceneName);
        addScene(scene);
        // setup scene camera
        Camera3D mainCamera("NewCamera", getAspectRatio(), scene.get());
        scene->setCamera(mainCamera);
        // setup skybox
        u32 skyboxId = TextureBuffer::load({
            { "assets/materials/skybox/front.jpg", TextureFaceType::FRONT },
            { "assets/materials/skybox/back.jpg", TextureFaceType::BACK },
            { "assets/materials/skybox/left.jpg", TextureFaceType::LEFT },
            { "assets/materials/skybox/right.jpg", TextureFaceType::RIGHT },
            { "assets/materials/skybox/top.jpg", TextureFaceType::TOP },
            { "assets/materials/skybox/bottom.jpg", TextureFaceType::BOTTOM }
        });
        TextureBuffer::setDefaultParamsCubeMap(skyboxId);
        auto skybox = SkyboxCube(
                "NewSkybox",
                scene.get(),
                CubeMapTextureComponent(skyboxId, TextureType::CUBE_MAP)
        );
        scene->setSkybox(skybox);
        RenderSystem::skyboxRenderer.upload(skybox.get<Skybox>());
        // setup HDR env
        auto hdrEnv = HdrEnvCube(
                "HdrEnvCube",
                scene.get(),
                TextureComponent(
                        TextureBuffer::load("assets/hdr/ice_lake.hdr", io::Spectrum::HDR),
                        TextureType::TEXTURE_2D,
                        IntUniform { "hdrEnv", 0 })
        );
        scene->setHdrEnv(hdrEnv);
        RenderSystem::hdrEnvRenderer.upload(hdrEnv.get<HdrEnv>());
        // setup light sources
        PhongLight("L_Sun_1", scene.get()).getPosition() = { -10, 10, -10 };
        PhongLight("L_Sun_2", scene.get()).getPosition() = { 10, 10, 10 };
        PhongLight("L_Sun_3", scene.get()).getPosition() = { -10, 10, 10 };
        PhongLight("L_Sun_4", scene.get()).getPosition() = { 10, 10, -10 };
        // setup geometry or mesh
        ThreadPoolScheduler->execute([this, scene]() {
            vector<Batch3d> batches = loadModel(scene);
            RenderScheduler->execute([batches]() {
                auto temp = batches;
                RenderSystem::batchRenderer->createVIRenderModel(temp);
            });
        });
        return scene;
    }

    Ref<Renderer> Application::createBatchRenderer() {
        auto entityHandler = [](
                ecs::Registry& registry,
                ecs::entity_id entityId,
                u32 index,
                BaseShaderProgram& shader
        ) {
            // pass entity id to shader
            auto* uuid = registry.getComponent<UUIDComponent>(entityId);
            IntUniform uuidUniform = { "uuids", uuid->uuid };
            shader.setUniformArrayElement(index, uuidUniform);
            ENGINE_INFO("UUID: {0}", uuid->uuid);
            // update single material
            auto material = registry.getComponent<Material>(entityId);
            if (material) {
                MaterialShader(shader).setMaterial(index, material);
            }
        };
        // setup batch and instanced renderers
        auto batchShader = shader::BaseShaderProgram(
                io::ShaderProps {
                        "batch",
                        "v_batch.glsl",
                        "scene_phong.glsl",
                        ENGINE_SHADERS_PATH
                },
                BaseShader(),
                BaseShader(),
                { camera3dUboScript(), lightScript() }
        );
        batchShader.setInstancesPerDraw(4);
        Ref<Renderer> batchRenderer = createRef<BatchRenderer<Vertex3d>>(batchShader);
        batchRenderer->addEntityHandler(entityHandler);
        RenderSystem::batchRenderer = batchRenderer;
        return batchRenderer;
    }

    Ref<Renderer> Application::createInstanceRenderer() {
        auto entityHandler = [](
                ecs::Registry& registry,
                ecs::entity_id entityId,
                u32 index,
                BaseShaderProgram& shader
        ) {
            // pass entity id to shader
            auto* uuid = registry.getComponent<UUIDComponent>(entityId);
            IntUniform uuidUniform = { "uuids", uuid->uuid };
            shader.setUniformArrayElement(index, uuidUniform);
            ENGINE_INFO("UUID: {0}", uuid->uuid);
            // update single material
            auto material = registry.getComponent<Material>(entityId);
            if (material) {
                MaterialShader(shader).setMaterial(index, material);
            }
        };
        auto instanceShader = shader::BaseShaderProgram(
                io::ShaderProps {
                        "instance",
                        "v_instance.glsl",
                        "scene_phong.glsl",
                        ENGINE_SHADERS_PATH
                },
                BaseShader(),
                BaseShader(),
                { camera3dUboScript(), lightScript() }
        );
        instanceShader.setInstancesPerDraw(4);
        Ref<Renderer> instanceRenderer = createRef<InstanceRenderer<Vertex3d>>(instanceShader);
        instanceRenderer->addEntityHandler(entityHandler);
        RenderSystem::instanceRenderer = instanceRenderer;
        return instanceRenderer;
    }

    void Application::bindCamera(Camera3D& camera) {
        KEY_PRESSED(KeyCode::W) { [&camera](KeyCode keycode) {
            camera.applyMove(MoveType::UP);
        }};
        KEY_PRESSED(KeyCode::A) { [&camera](KeyCode keycode) {
            camera.applyMove(MoveType::LEFT);
        }};
        KEY_PRESSED(KeyCode::S) { [&camera](KeyCode keycode) {
            camera.applyMove(MoveType::DOWN);
        }};
        KEY_PRESSED(KeyCode::D) { [&camera](KeyCode keycode) {
            camera.applyMove(MoveType::RIGHT);
        }};
        KEY_PRESSED(KeyCode::Q) { [&camera](KeyCode keycode) {
            camera.applyRotate(RotateType::LEFT_Z);
        }};
        KEY_PRESSED(KeyCode::E) { [&camera](KeyCode keycode) {
            camera.applyRotate(RotateType::RIGHT_Z);
        }};
        KEY_PRESSED(KeyCode::Z) { [&camera](KeyCode keycode) {
            camera.applyZoom(ZoomType::ZOOM_IN);
        }};
        KEY_PRESSED(KeyCode::X) { [&camera](KeyCode keycode) {
            camera.applyZoom(ZoomType::ZOOM_OUT);
        }};
        camera.zoomSpeed = 10.0f;
        camera.rotateSpeed = 0.5f;
        camera.moveSpeed = 0.5f;
        camera.distance = 10.0f;

        GAMEPAD_PRESSED(GamepadButtonCode::PAD_BTN_A) { [this](GamepadButtonCode gamepadBtnCode) {
            onPadA();
        }};
        GAMEPAD_PRESSED(GamepadButtonCode::PAD_BTN_B) { [this](GamepadButtonCode gamepadBtnCode) {
            onPadB();
        }};
        GAMEPAD_PRESSED(GamepadButtonCode::PAD_BTN_X) { [this](GamepadButtonCode gamepadBtnCode) {
            onPadX();
        }};
        GAMEPAD_PRESSED(GamepadButtonCode::PAD_BTN_Y) { [this](GamepadButtonCode gamepadBtnCode) {
            onPadY();
        }};

        GAMEPAD_ROLL_LEFT() { [this](const GamepadRoll& roll) {
            onGamepadRollLeft(roll);
        }};
        GAMEPAD_ROLL_RIGHT() { [this](const GamepadRoll& roll) {
            onGamepadRollRight(roll);
        }};
    }

    void Application::onPadA() {
        RUNTIME_INFO("Gamepad button A pressed!");
        if (activeScene) {
            auto& camera = activeScene->getCamera();
            camera.move(DOWN);
            camera.applyView();
        }
    }

    void Application::onPadB() {
        RUNTIME_INFO("Gamepad button B pressed!");
        if (activeScene) {
            auto& camera = activeScene->getCamera();
            camera.move(RIGHT);
            camera.applyView();
        }
    }

    void Application::onPadX() {
        RUNTIME_INFO("Gamepad button X pressed!");
        if (activeScene) {
            auto& camera = activeScene->getCamera();
            camera.move(LEFT);
            camera.applyView();
        }
    }

    void Application::onPadY() {
        RUNTIME_INFO("Gamepad button Y pressed!");
        if (activeScene) {
            auto& camera = activeScene->getCamera();
            camera.move(UP);
            camera.applyView();
        }
    }

    void Application::onGamepadRollLeft(const GamepadRoll& roll) {
        RUNTIME_INFO("onGamepadRollLeft: (x: {0}, y: {1}, trigger: {2})", roll.x, roll.y, roll.triggered);
        if (activeScene) {
            auto& camera = activeScene->getCamera();
            camera.move({ roll.x, roll.y, 1 });
            camera.applyView();
        }
    }

    void Application::onGamepadRollRight(const GamepadRoll& roll) {
        RUNTIME_INFO("onGamepadRollRight: (x: {0}, y: {1}, trigger: {2})", roll.x, roll.y, roll.triggered);
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