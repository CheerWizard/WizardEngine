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
            update();
        }
        onDestroy();
    }

    void Application::onCreate() {
        PROFILE_FUNCTION();
        ENGINE_INFO("onCreate()");
        jobSystem = createScope<JobSystem<>>();
        // setup window, input, graphics
        RenderScheduler->execute([this]() {
            if (!ProjectProps::createFromFile("properties.yaml", projectProps)) {
                ENGINE_WARN("Application: Unable to create properties from properties.yaml");
            }
            m_Window = createScope<Window>(projectProps.windowProps);
            // set keycodes mapping
            loadGamepadMappings("assets/mappings/game_controller.txt");
            // set window icon
            io::TextureFile::setFlipTexture(false);
            if (!projectProps.icon.empty()) {
                setWindowIcon(projectProps.icon);
            }
            // init renderers
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
            graphics::enableSRGB();
            // setup ImGui
#ifdef VISUAL
            Visual::init(getNativeWindow());
            Visual::fullScreen = projectProps.windowProps.fullscreen;
            Visual::openDockspace = projectProps.windowProps.dockspace;
            AssetBrowser::create(getNativeWindow());
            MaterialPanel::create(getNativeWindow());
            onVisualCreate();
#endif
        });
        // setup audio
        AudioScheduler->execute([]() {
            audio::DeviceManager::createContext();
        });
        // setup network
        NetworkScheduler->execute([]() {
        });
        // setup something heavy
        ThreadPoolScheduler->execute([this]() {
            if (projectProps.launcher != "") {
                setActiveScene(io::LocalAssetManager::read(projectProps.launcher.c_str()));
            } else {
                ENGINE_WARN("Application: Launcher scene not specified in properties.yaml!");
            }
        });
        RenderScheduler->execute([this]() {
            m_Window->onPrepare();
            m_Window->setInCenter();
            setSampleSize(projectProps.windowProps.sampleSize);
        });
        waitAllJobs();
        Input::create(m_Window->getNativeWindow());
    }

    void Application::onVisualCreate() {
    }

    void Application::onDestroy() {
        PROFILE_FUNCTION();
        ENGINE_INFO("onDestroy()");
        RenderScheduler->execute([]() {
            RenderSystem::onDestroy();
#ifdef VISUAL
            Visual::release();
#endif
        });
        AudioScheduler->execute([]() {
            audio::MediaPlayer::clear();
            audio::DeviceManager::clear();
        });
        ScriptSystem::onDestroy();
        waitAllJobs();
    }

    void Application::update() {
        PROFILE_FUNCTION();
        Timer timer("Application::update()", 30);
        // update simulation systems
        ThreadPoolScheduler->execute([this]() {
            onSimulationUpdate();
        });
        // update render systems
        RenderScheduler->execute([this]() {
            if (activeScene && !activeScene->isEmpty()) {
                RenderSystem::onUpdate();
            }
            // update ImGui tools
#ifdef VISUAL
            Visual::begin();
            Visual::onUpdate(dt);
            onVisualDraw();
            Visual::end();
#endif
        });
        // update monitor and inputs
        if (enableMouseCursor) {
            Input::updateMousePosition();
        }
        onEventUpdate();
        RenderScheduler->execute([this]() {
            m_Window->onUpdate();
        });
        // sync simulation with delta time
        RenderScheduler->wait();
        ThreadPoolScheduler->wait();
        dt = timer.stop();
        PROFILE_ON_FRAME_UPDATED();
    }

    void Application::onUpdate() {
    }

    void Application::onWindowClosed() {
        ENGINE_INFO("Application : onWindowClosed()");
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
        EventRegistry::onKeyPressedMap[keyCode].function(keyCode);
    }

    void Application::onKeyHold(event::KeyCode keyCode) {
        PROFILE_FUNCTION();
#ifdef VISUAL
        Visual::onKeyHold(keyCode);
        if (Visual::blocksKeyboard())
            return;
#endif
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
        EventRegistry::onMouseScrolled.function(xOffset, yOffset);
    }

    void Application::onCursorMoved(double xPos, double yPos) {
        PROFILE_FUNCTION();
#ifdef VISUAL
        Visual::onCursorMoved(xPos, yPos);
        if (Visual::blocksMousePos())
            return;
#endif
        EventRegistry::onCursorMoved.function(xPos, yPos);
    }

    void Application::onKeyTyped(event::KeyCode keyCode) {
        PROFILE_FUNCTION();
#ifdef VISUAL
        Visual::onKeyTyped(keyCode);
        if (Visual::blocksTextInput())
            return;
#endif
    }

    float Application::getAspectRatio() const {
        return m_Window->getAspectRatio();
    }

    void Application::setActiveScene(const Ref<Scene>& scene) {
        PROFILE_FUNCTION();
        LocalAssetManager::setScene(scene);
        activeScene = scene;
        selectedEntity = Entity(activeScene.get());
        hoveredEntity = Entity(activeScene.get());
        bindCamera(activeScene->getCamera());
        RenderSystem::activeScene = activeScene;
        ScriptSystem::activeScene = activeScene;
        Physics::activeScene = activeScene;
    }

    void Application::restart() {
        onDestroy();
        onCreate();
    }

    int Application::getWindowWidth() {
        return m_Window->getWidth();
    }

    int Application::getWindowHeight() {
        return m_Window->getHeight();
    }

    int Application::getRefreshRate() {
        return m_Window->getRefreshRate();
    }

    void *Application::getNativeWindow() {
        return m_Window->getNativeWindow();
    }

    void Application::setWindowIcon(const std::string &filePath) {
        m_Window->setWindowIcon(filePath);
    }

    Ref<tools::FileDialog> Application::createFileDialog() {
        return createRef<tools::FileDialog>(m_Window->getNativeWindow());
    }

    void Application::setSampleSize(int samples) {
        m_Window->setSampleSize(samples);
        // update active scene frame format
        RenderScheduler->execute([this, samples]() {
            FrameBufferFormat activeSceneFrameFormat;
            activeSceneFrameFormat.colorAttachments = {
                    { ColorFormat::RGBA8, ColorFormat::RGBA },
                    { ColorFormat::RGBA8, ColorFormat::RGBA },
                    { ColorFormat::RED_I32, ColorFormat::RED_INTEGER }
            };
            activeSceneFrameFormat.renderBufferAttachment = { DepthStencilFormat::DEPTH24STENCIL8 };
            activeSceneFrameFormat.width = m_Window->getWidth();
            activeSceneFrameFormat.height = m_Window->getHeight();
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
            onWindowResized(m_Window->getWidth(), m_Window->getHeight());
        });
    }

    void Application::createGraphics() {
        graphics::initContext(m_Window->getNativeWindow());
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

    void Application::onSimulationUpdate() {
        if (activeScene && !activeScene->isEmpty()) {
            Physics::onUpdate(dt);
            ScriptSystem::onUpdate(dt);
            onUpdate();

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
        m_Window->loadGamepadMappings(mappingsFilePath);
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
        hdrFrameFormat.width = m_Window->getWidth();
        hdrFrameFormat.height = m_Window->getHeight();
        RenderSystem::hdrEffectRenderer = { hdrFrameFormat };
        hdrEffect = RenderSystem::hdrEffectRenderer.getHdrEffect();
    }

    void Application::initBlur() {
        FrameBufferFormat blurFrameFormat;
        blurFrameFormat.colorAttachments = {
                { ColorFormat::RGBA8, ColorFormat::RGBA },
        };
        blurFrameFormat.width = m_Window->getWidth();
        blurFrameFormat.height = m_Window->getHeight();
        BlurEffectRenderer blurEffectRenderer(blurFrameFormat);
        blurEffect = blurEffectRenderer.getBlurEffect();
        RenderSystem::postEffectRenderers.emplace_back(blurEffectRenderer);
    }

    void Application::initSharpen() {
        FrameBufferFormat sharpenFrameFormat;
        sharpenFrameFormat.colorAttachments = {
                { ColorFormat::RGBA8, ColorFormat::RGBA },
        };
        sharpenFrameFormat.width = m_Window->getWidth();
        sharpenFrameFormat.height = m_Window->getHeight();
        SharpenEffectRenderer sharpenEffectRenderer(sharpenFrameFormat);
        sharpenEffect = sharpenEffectRenderer.getSharpenEffect();
        RenderSystem::postEffectRenderers.emplace_back(sharpenEffectRenderer);
    }

    void Application::initEdgeDetection() {
        FrameBufferFormat edgeDetectionFrameFormat;
        edgeDetectionFrameFormat.colorAttachments = {
                { ColorFormat::RGBA8, ColorFormat::RGBA },
        };
        edgeDetectionFrameFormat.width = m_Window->getWidth();
        edgeDetectionFrameFormat.height = m_Window->getHeight();
        EdgeDetectionEffectRenderer edgeDetectionEffectRenderer(edgeDetectionFrameFormat);
        edgeDetectionEffect = edgeDetectionEffectRenderer.getEdgeDetectionEffect();
        RenderSystem::postEffectRenderers.emplace_back(edgeDetectionEffectRenderer);
    }

    void Application::initGaussianBlur() {
        FrameBufferFormat gaussianBlurFrameFormat;
        gaussianBlurFrameFormat.colorAttachments = {
                { ColorFormat::RGBA8, ColorFormat::RGBA },
        };
        gaussianBlurFrameFormat.width = m_Window->getWidth();
        gaussianBlurFrameFormat.height = m_Window->getHeight();
        GaussianBlurEffectRenderer gaussianBlurEffectRenderer(gaussianBlurFrameFormat);
        gaussianBlurEffect = gaussianBlurEffectRenderer.getGaussianBlurEffect();
        RenderSystem::gaussianBlurRenderer = gaussianBlurEffectRenderer;
    }

    void Application::initTextureMixer() {
        FrameBufferFormat textureMixerFrameFormat;
        textureMixerFrameFormat.colorAttachments = {
                { ColorFormat::RGBA8, ColorFormat::RGBA },
        };
        textureMixerFrameFormat.width = m_Window->getWidth();
        textureMixerFrameFormat.height = m_Window->getHeight();
        RenderSystem::textureMixer = { textureMixerFrameFormat };
    }

    void Application::onVisualDraw() {
        // this method will be called only from derived class that defines VISUAL macro
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
        return loadModel(LocalAssetManager::getScene(sceneId));
    }

    Ref<Scene> Application::newScene(const std::string& sceneName) {
        auto scene = createRef<Scene>(sceneName);

        ThreadPoolScheduler->execute([this, scene]() {
            // setup camera
            LocalAssetManager::addScene(scene);
            Camera3D mainCamera("NewCamera", getAspectRatio(), scene.get());
            scene->setCamera(mainCamera);
            // setup light sources
            PhongLight("L_Sun_1", scene.get()).getPosition() = { -10, 10, -10 };
            PhongLight("L_Sun_2", scene.get()).getPosition() = { 10, 10, 10 };
            PhongLight("L_Sun_3", scene.get()).getPosition() = { -10, 10, 10 };
            PhongLight("L_Sun_4", scene.get()).getPosition() = { 10, 10, -10 };
            // setup skybox
            std::vector<std::pair<u32, TextureData>> textures = {
                    { TextureFaceType::FRONT, TextureFile::read("assets/materials/skybox/front.jpg") },
                    { TextureFaceType::BACK, TextureFile::read("assets/materials/skybox/back.jpg") },
                    { TextureFaceType::RIGHT, TextureFile::read("assets/materials/skybox/right.jpg") },
                    { TextureFaceType::LEFT, TextureFile::read("assets/materials/skybox/left.jpg") },
                    { TextureFaceType::BOTTOM, TextureFile::read("assets/materials/skybox/bottom.jpg") },
                    { TextureFaceType::TOP, TextureFile::read("assets/materials/skybox/top.jpg") },
            };
            RenderScheduler->execute([scene, &textures]() {
                u32 skyboxId = TextureBuffer::upload(textures);
                auto skybox = SkyboxCube(
                        "NewSkybox",
                        scene.get(),
                        CubeMapTextureComponent(skyboxId, TextureType::CUBE_MAP)
                );
                TextureBuffer::setDefaultParamsCubeMap(skyboxId);
                scene->setSkybox(skybox);
                auto* sky_box = skybox.get<Skybox>();
                RenderSystem::skyboxRenderer.upload(sky_box);
            });
            // setup HDR env
            TextureData td = TextureFile::read("assets/hdr/ice_lake.hdr", io::Spectrum::HDR);
            RenderScheduler->execute([scene, &td]() {
                auto hdrEnv = HdrEnvCube(
                        "HdrEnvCube",
                        scene.get(),
                        TextureComponent(
                                TextureBuffer::upload(td),
                                TextureType::TEXTURE_2D,
                                IntUniform { "hdrEnv", 0 })
                );
                scene->setHdrEnv(hdrEnv);
                auto* hdr_env = hdrEnv.get<HdrEnv>();
                RenderSystem::hdrEnvRenderer.upload(hdr_env);
            });
            // setup geometry or mesh
            vector<Batch3d> batches = loadModel(scene);
            RenderScheduler->execute([&batches]() {
                RenderSystem::batchRenderer->createVIRenderModel(batches);
            });
            // wait while render thread uploads data and clean it
            RenderScheduler->wait();
            TextureFile::free(td.pixels);
            for (const auto& texture : textures) {
                TextureFile::free(texture.second.pixels);
            }
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
                ENGINE_SHADERS_PATH + "/" + "v_batch.glsl",
                ENGINE_SHADERS_PATH + "/" + "scene_phong.glsl",
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
                ENGINE_SHADERS_PATH + "/" + "v_instance.glsl",
                ENGINE_SHADERS_PATH + "/" + "scene_phong.glsl",
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