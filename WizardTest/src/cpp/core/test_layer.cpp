//
// Created by mecha on 11.04.2022.
//

#include <core/test_layer.h>

namespace test {

    void TestLayer::init() {
        auto scene = createRef<Scene>();
        auto& app = engine::core::Application::get();
        app.setActiveScene(scene);

        auto camera = Camera3D {
                "SceneCamera",
                app.getAspectRatio(),
                scene.get()
        };

        cameraController = createRef<Camera3dController>("CameraController", camera);

        u32 skyboxId = TextureBuffer::load(
                {
                        { "assets/textures/skybox/front.jpg", TextureFaceType::FRONT },
                        { "assets/textures/skybox/back.jpg", TextureFaceType::BACK },
                        { "assets/textures/skybox/left.jpg", TextureFaceType::LEFT },
                        { "assets/textures/skybox/right.jpg", TextureFaceType::RIGHT },
                        { "assets/textures/skybox/top.jpg", TextureFaceType::TOP },
                        { "assets/textures/skybox/bottom.jpg", TextureFaceType::BOTTOM },
                }
        );
        TextureBuffer::setDefaultParamsCubeMap(skyboxId);

        scene->setSkybox(SkyboxCube(
                "Skybox",
                scene.get(),
                CubeMapTextureComponent(skyboxId, TextureBuffer::getTypeId(TextureType::CUBE_MAP))
        ));

        survivalBackPack = Object3d<BatchVertex<Vertex3d>>(
                "SurvivalBackPack",
                scene.get()
        );

        try {
            auto model = io::ModelFile::read("assets/model/DesertEagle.fbx");
            auto mesh = toMesh3dBatch(model.meshComponent);
            survivalBackPack.add<BaseMeshComponent<BatchVertex<Vertex3d>>>(mesh);
        } catch (const file_not_found& ex) {
            RUNTIME_ERR("Could not load model file!");
        }

        audio::MediaPlayer::loadStream(
                "assets/audio/forest.wav",
                {},
                [](const audio::Source &source) {
                    audio::MediaPlayer::setPlayedSource(source);
                },
                []() {
                    audio::MediaPlayer::playStream();
                }
        );

        TCP_CLIENT_INIT(this);
        TCP_CLIENT_CONNECT_RUN(localhost, 54000);

        UDP_CLIENT_INIT(this);
        UDP_CLIENT_BIND(localhost, 54000);
    }

    TestLayer::~TestLayer() {
        TCP_CLIENT_CLOSE();
        UDP_CLIENT_CLOSE();
    }

    void TestLayer::bindCamera() {
        cameraController->bind(event::KeyCode::W, MoveType::UP);
        cameraController->bind(event::KeyCode::A, MoveType::LEFT);
        cameraController->bind(event::KeyCode::S, MoveType::DOWN);
        cameraController->bind(event::KeyCode::D, MoveType::RIGHT);
        cameraController->bind(event::KeyCode::Q, RotateType::LEFT_Z);
        cameraController->bind(event::KeyCode::E, RotateType::RIGHT_Z);
        cameraController->bind(event::KeyCode::Z, ZoomType::ZOOM_IN);
        cameraController->bind(event::KeyCode::X, ZoomType::ZOOM_OUT);
    }

    void TestLayer::onPrepare() {
        bindCamera();

        BIND_GAMEPAD_BUTTON_PRESSED(GamepadButtonCode::PAD_BTN_A, onPadA(););
        BIND_GAMEPAD_BUTTON_PRESSED(GamepadButtonCode::PAD_BTN_B, onPadB(););
        BIND_GAMEPAD_BUTTON_PRESSED(GamepadButtonCode::PAD_BTN_X, onPadX(););
        BIND_GAMEPAD_BUTTON_PRESSED(GamepadButtonCode::PAD_BTN_Y, onPadY(););

        BIND_KEY_PRESSED(KeyCode::D1, audio::MediaPlayer::pause(););
        BIND_KEY_PRESSED(KeyCode::D2, audio::MediaPlayer::stop(););
        BIND_KEY_PRESSED(KeyCode::D3, audio::MediaPlayer::playStream(););
    }

    void TestLayer::onUpdate(time::Time deltaTime) {
        cameraController->setDeltaTime(deltaTime);
        // rotate survival backpack
        auto& modelMatrix = survivalBackPack.get<Transform3dComponent>()->modelMatrix;
        modelMatrix.rotation.y += 0.005f;
        updateModel3d(modelMatrix);

        UDP_CLIENT_SEND("Hello from UDP!");
    }

    void TestLayer::onWindowResized(const uint32_t &width, const uint32_t &height) {
        cameraController->onWindowResized(width, height);
    }

    void TestLayer::onKeyPressed(event::KeyCode keyCode) {
        cameraController->onKeyPressed(keyCode);
    }

    void TestLayer::onKeyHold(event::KeyCode keyCode) {
        cameraController->onKeyHold(keyCode);
    }

    void TestLayer::onKeyReleased(event::KeyCode keyCode) {
        cameraController->onKeyReleased(keyCode);
    }

    void TestLayer::onKeyTyped(event::KeyCode keyCode) {
        cameraController->onKeyTyped(keyCode);
    }

    void TestLayer::onPadA() {
        ENGINE_INFO("Gamepad button A pressed!");
        cameraController->applyMove(MoveType::DOWN);
    }

    void TestLayer::onPadB() {
        ENGINE_INFO("Gamepad button B pressed!");
        cameraController->applyMove(MoveType::RIGHT);
    }

    void TestLayer::onPadX() {
        ENGINE_INFO("Gamepad button X pressed!");
        cameraController->applyMove(MoveType::LEFT);
    }

    void TestLayer::onPadY() {
        ENGINE_INFO("Gamepad button Y pressed!");
        cameraController->applyMove(MoveType::UP);
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

    void TestLayer::tcp_socketNotCreated() {

    }

    void TestLayer::tcp_connectionFailed() {

    }

    void TestLayer::tcp_socketClosed() {

    }

    void TestLayer::udp_socketNotCreated() {

    }

    void TestLayer::udp_sendDataFailed(const std::string &data) {

    }

    void TestLayer::udp_socketClosed() {

    }
}