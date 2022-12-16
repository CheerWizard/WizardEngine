//
// Created by mecha on 11.04.2022.
//

#include <core/test_layer.h>
#include <graphics/camera/CameraShaderScript.h>

#define ROTATE_SURVIVAL_PACK 123

namespace test {

    void TestLayer::init() {
        // todo fix hdr cube map conversion
//        Application::get().setHdrEnvCube(scene, "assets/hdr/ice_lake.hdr");
    }

    TestLayer::~TestLayer() {
        AssetBrowser::destroy();
    }

    void TestLayer::onPrepare() {
        KEY_PRESSED(D1) { [this](KeyCode keycode) {
            switchHDR();
        }};

        KEY_PRESSED(D2) { [this](KeyCode keycode) {
            switchBlur();
        }};

        KEY_PRESSED(D3) { [this](KeyCode keycode) {
            switchSharpen();
        }};

        KEY_PRESSED(D4) { [this](KeyCode keycode) {
            switchEdgeDetection();
        }};

        KEY_PRESSED(D5) { [this](KeyCode keycode) {
            switchBloom();
        }};

        KEY_PRESSED(D9) { [](KeyCode keycode) {
            Application::get().activeSceneFrame->setRenderTargetIndex(0);
        }};

        KEY_PRESSED(D0) { [](KeyCode keycode) {
            Application::get().activeSceneFrame->setRenderTargetIndex(1);
        }};

        KEY_PRESSED(M) { [this](KeyCode keycode) {
            switchMSAA();
        }};

//        KEY_PRESSED(KeyCode::D1, audio::MediaPlayer::pause(););
//        KEY_PRESSED(KeyCode::D2, audio::MediaPlayer::stop(););
//        KEY_PRESSED(KeyCode::D3, audio::MediaPlayer::playStream(););

//        KEY_PRESSED(
//                KeyCode::R,
//                // notify all players to rotate a survival pack
//                GDHeader header(CLIENT_TO_CLIENT, ROTATE_SURVIVAL_PACK);
//                GDFloat rotation(0.5f);
//                udp::Client::getRequestQueue().push(header, rotation);
//        );

//        KEY_PRESSED(
//                KeyCode::K,
//                io::RemoteAssetManager::saveScene(Application::get().activeScene);
//        );
//
//        KEY_PRESSED(
//                KeyCode::L,
//                io::RemoteAssetManager::loadScene(Application::get().activeScene->getName().c_str());
//        );
        Application::get().enableMouseHovering = true;
        Application::get().enableMouseCursor = true;
    }

    void TestLayer::onUpdate(time::Time dt) {
        // notify all players to rotate a survival pack
//        GDHeader header(CLIENT_TO_CLIENT, ROTATE_SURVIVAL_PACK);
//        GDFloat rotation(0.005f);
//        udp::Client::get().getRequestQueue().push(header, rotation);
//        tcp::Client::get().getRequestQueue().push(header, rotation);

//        Application::get().activeScene->getSkybox().get<Skybox>()->rotate({0, 0.001f, 0});

//        auto hoveredTransform = Application::get().hoveredEntity.get<Transform3dComponent>();
//        if (hoveredTransform) {
//            ENGINE_INFO("hoveredEntity: transform");
//            hoveredTransform->modelMatrix.rotation[1] += 0.1f;
//            hoveredTransform->modelMatrix.apply();
//        }
    }

    void TestLayer::onWindowResized(const uint32_t &width, const uint32_t &height) {
        RUNTIME_INFO("TestLayer::onWindowResized(w: {0}, h: {1})", width, height);
    }

    void TestLayer::onKeyPressed(event::KeyCode keyCode) {
    }

    void TestLayer::onKeyHold(event::KeyCode keyCode) {
    }

    void TestLayer::onKeyReleased(event::KeyCode keyCode) {
    }

    void TestLayer::onKeyTyped(event::KeyCode keyCode) {
    }

    void TestLayer::onWindowClosed() {
    }

    void TestLayer::onMousePressed(event::MouseCode mouseCode) {

    }

    void TestLayer::onMouseRelease(event::MouseCode mouseCode) {

    }

    void TestLayer::onMouseScrolled(double xOffset, double yOffset) {

    }

    void TestLayer::onCursorMoved(double xPos, double yPos) {

    }

    void TestLayer::onTCPSocketCreated() {

    }

    void TestLayer::onTCPConnectionFailed() {

    }

    void TestLayer::onTCPConnected() {

    }

    void TestLayer::onTCPSocketClosed() {

    }

    void TestLayer::onTCPSenderFailed(char *data, size_t size) {

    }

    void TestLayer::onTCPSenderSuccess() {

    }

    void TestLayer::onTCPReceiverFailed(char *data, size_t size) {

    }

    void TestLayer::onTCPReceiverSuccess(const YAML::Node &gdNode, const GDHeader &header) {
        RUNTIME_INFO("onTCPReceiverSuccess()");
        RUNTIME_INFO("GDHeader[address: {0}, type: {1}]", header.address, header.type);
        io::RemoteAssetManager::dispatchScene(Application::get().activeScene, gdNode, header.type);
        // dispatch header type
        if (header.type == ROTATE_SURVIVAL_PACK) {
            // unpack data
            GDPrimitive<f32> gdPrimitive;
            gdPrimitive.deserialize(gdNode);
            auto rotationY = gdPrimitive.value;
            RUNTIME_INFO("Rotating SurvivalPack with value: {0}", rotationY);
            // handle data
//            auto& modelMatrix = survivalBackPack.get<Transform3dComponent>()->modelMatrix;
//            modelMatrix.rotation.v[1] += rotationY;
//            updateModel3d(modelMatrix);
        }
    }

    void TestLayer::onUDPSocketCreated() {

    }

    void TestLayer::onUDPSocketClosed() {

    }

    void TestLayer::onUDPSenderFailed(char *data, size_t size) {

    }

    void TestLayer::onUDPSenderSuccess() {

    }

    void TestLayer::onUDPReceiverFailed(char *data, size_t size) {

    }

    void TestLayer::onUDPReceiverSuccess(const YAML::Node &gdNode, const GDHeader &header) {
        RUNTIME_INFO("onUDPReceiverSuccess()");
        RUNTIME_INFO("GDHeader[address: {0}, type: {1}]", header.address, header.type);
        io::RemoteAssetManager::dispatchScene(Application::get().activeScene, gdNode, header.type);
        // dispatch header type
        if (header.type == ROTATE_SURVIVAL_PACK) {
            // unpack data
            GDFloat rotation;
            rotation.deserialize(gdNode);
            auto rotationY = rotation.value;
            RUNTIME_INFO("Rotating SurvivalPack with value: {0}", rotationY);
            // handle data
//            auto& modelMatrix = survivalBackPack.get<Transform3dComponent>()->modelMatrix;
//            modelMatrix.rotation.v[1] += rotationY;
//            updateModel3d(modelMatrix);
        }
    }

    void TestLayer::switchMSAA() {
        msaaEnabled = !msaaEnabled;
        Application::get().setSampleSize(msaaEnabled ? 8 : 1);
    }

    void TestLayer::switchHDR() {
        Application::get().hdrEffect->enabled = !Application::get().hdrEffect->enabled;
    }

    void TestLayer::switchBlur() {
        Application::get().blurEffect->enabled = !Application::get().blurEffect->enabled;
    }

    void TestLayer::switchSharpen() {
        Application::get().sharpenEffect->enabled = !Application::get().sharpenEffect->enabled;
    }

    void TestLayer::switchEdgeDetection() {
        Application::get().edgeDetectionEffect->enabled = !Application::get().edgeDetectionEffect->enabled;
    }

    void TestLayer::switchGaussianBlur() {
        Application::get().gaussianBlurEffect->enabled = !Application::get().gaussianBlurEffect->enabled;
    }

    void TestLayer::switchBloom() {
        switchHDR();
        switchGaussianBlur();
    }

    void TestLayer::onVisualDraw(time::Time dt) {
        Visual::drawDockspace();
        // toolbar
        Toolbar::get().draw();
//        // Scene hierarchy and properties
        sceneHierarchy.onUpdate(dt);
//        // Scene view port
        sceneViewport.setRenderTarget(RenderSystem::finalRenderTargetId);
        sceneViewport.onUpdate(dt);
//        // Console
        Console::get().draw(1024, 768);
//        // Troubleshoot
        ProfilerMenu::draw(1024, 768);
//        // Projects storage
        ProjectsPanel::get().draw();
//        // Assets explorer
        AssetBrowser::draw(dt);
//        // Logger
        Log::get().draw();
     }

    void TestLayer::updateGizmo() {
//        if (EventRegistry::keyHold(KeyCode::LeftControl) && Input::isMousePressed(MouseCode::ButtonLeft)) {
//            Entity selectedEntity = Application::get().hoveredEntity;
//            auto* selectedTransform = selectedEntity.get<Transform3dComponent>();
//            if (selectedTransform) {
//                Application::get().selectedEntity = selectedEntity;
//                showGizmo = !showGizmo;
//            }
//        }
//
//        if (showGizmo) {
//            // get selected entity transform
//            auto& app = Application::get();
//            Entity selectedEntity = app.selectedEntity;
//            auto* selectedTransform = selectedEntity.get<Transform3dComponent>();
//            // get window position and size
//            vec2f windowSize = {
//                    static_cast<float>(app.getWindowWidth()),
//                    static_cast<float>(app.getWindowHeight())
//            };
//            int xPos = 0, yPos = 0;
//            app.getWindow()->getPosition(xPos, yPos);
//            // draw translation gizmo
//            Gizmo::drawTranslate(mainCamera, *selectedTransform,
//                                 { static_cast<float>(xPos), static_cast<float>(yPos) },
//                                 windowSize);
//        }
    }

    void TestLayer::onSceneRemoved(const Ref<Scene> &scene) {
    }

    void TestLayer::onEntityRemoved(const Entity &entity) {
    }

    void TestLayer::onSceneSelected(const Ref<Scene> &scene) {
        Application::get().setActiveScene(scene);
        sceneViewport.setScene(scene);
    }

    void TestLayer::onEntitySelected(const Entity &entity) {

    }
}