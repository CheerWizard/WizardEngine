//
// Created by mecha on 11.04.2022.
//

#include <core/test_layer.h>

#define ROTATE_SURVIVAL_PACK 123

namespace test {

    void TestLayer::init() {
        auto scene = createRef<Scene>("Scene1");
        auto& app = engine::core::Application::get();
        app.setActiveScene(scene);

        mainCamera = Camera3D {
                "SceneCamera",
                app.getAspectRatio(),
                scene.get()
        };

        u32 skyboxId = TextureBuffer::load(
                {
                        { "assets/materials/skybox/front.jpg", TextureFaceType::FRONT },
                        { "assets/materials/skybox/back.jpg", TextureFaceType::BACK },
                        { "assets/materials/skybox/left.jpg", TextureFaceType::LEFT },
                        { "assets/materials/skybox/right.jpg", TextureFaceType::RIGHT },
                        { "assets/materials/skybox/top.jpg", TextureFaceType::TOP },
                        { "assets/materials/skybox/bottom.jpg", TextureFaceType::BOTTOM },
                }
        );
        TextureBuffer::setDefaultParamsCubeMap(skyboxId);

        scene->setSkybox(SkyboxCube(
                "Skybox",
                scene.get(),
                CubeMapTextureComponent(skyboxId, TextureBuffer::getTypeId(TextureType::CUBE_MAP))
        ));

        car = Object3d<BatchVertex<Vertex3d>>(
                "SurvivalBackPack",
                scene.get()
        );

        io::ModelFile<BatchVertex<Vertex3d>>::read("assets/model/porsche_911_gt2.obj", {
            [this](const BaseMeshComponent<BatchVertex<Vertex3d>>& mesh) {
                car.add<BaseMeshComponent<BatchVertex<Vertex3d>>>(mesh);
            },
            [](const exception& exception) {
                RUNTIME_EXCEPT(exception);
            },
            [](const io::ModelVertex& modelVertex) {
                return BatchVertex<Vertex3d> {
                    modelVertex.position,
                    modelVertex.uv,
                    modelVertex.normal,
                    0
                };
            }
        });

//        Cube<BatchVertex<Vertex3d>> cube;
//        Object3d<BatchVertex<Vertex3d>> cubeObj {
//            "Cube", scene.get()
//        };
//        cubeObj.add<VertexDataComponent<BatchVertex<Vertex3d>>>(cube);

//        audio::MediaPlayer::loadStream(
//                "assets/audio/forest.wav",
//                {},
//                [](const audio::Source &source) {
//                    audio::MediaPlayer::setPlayedSource(source);
//                },
//                []() {
//                    audio::MediaPlayer::playStream();
//                }
//        );

        bool tcpClientCreated = tcp::Client::init(this, this, this);
        if (tcpClientCreated) {
            tcp::Client::connect("192.168.1.101", 54000);
        }

        bool udpClientCreated = udp::Client::init(this, this, this);
        if (udpClientCreated) {
            udp::Client::connect("192.168.1.101", 54000);
        }
    }

    TestLayer::~TestLayer() {
        tcp::Client::close();
        udp::Client::close();
    }

    void TestLayer::bindCamera() {
        KEY_PRESSED(KeyCode::W, mainCamera.applyMove(MoveType::UP););
        KEY_PRESSED(KeyCode::A, mainCamera.applyMove(MoveType::LEFT););
        KEY_PRESSED(KeyCode::S, mainCamera.applyMove(MoveType::DOWN););
        KEY_PRESSED(KeyCode::D, mainCamera.applyMove(MoveType::RIGHT););
        KEY_PRESSED(KeyCode::Q, mainCamera.applyRotate(RotateType::LEFT_Z););
        KEY_PRESSED(KeyCode::E, mainCamera.applyRotate(RotateType::RIGHT_Z););
        KEY_PRESSED(KeyCode::Z, mainCamera.applyZoom(ZoomType::ZOOM_IN););
        KEY_PRESSED(KeyCode::X, mainCamera.applyZoom(ZoomType::ZOOM_OUT););

        KEY_HOLD(KeyCode::W, mainCamera.applyMove(MoveType::UP););
        KEY_HOLD(KeyCode::A, mainCamera.applyMove(MoveType::LEFT););
        KEY_HOLD(KeyCode::S, mainCamera.applyMove(MoveType::DOWN););
        KEY_HOLD(KeyCode::D, mainCamera.applyMove(MoveType::RIGHT););
        KEY_HOLD(KeyCode::Q, mainCamera.applyRotate(RotateType::LEFT_Z););
        KEY_HOLD(KeyCode::E, mainCamera.applyRotate(RotateType::RIGHT_Z););
        KEY_HOLD(KeyCode::Z, mainCamera.applyZoom(ZoomType::ZOOM_IN););
        KEY_HOLD(KeyCode::X, mainCamera.applyZoom(ZoomType::ZOOM_OUT););

        mainCamera.zoomSpeed = 10.0f;
        mainCamera.rotateSpeed = 0.5f;
        mainCamera.moveSpeed = 0.5f;
        mainCamera.distance = 10.0f;
    }

    void TestLayer::onPrepare() {
        bindCamera();

        GAMEPAD_PRESSED(GamepadButtonCode::PAD_BTN_A, onPadA(););
        GAMEPAD_PRESSED(GamepadButtonCode::PAD_BTN_B, onPadB(););
        GAMEPAD_PRESSED(GamepadButtonCode::PAD_BTN_X, onPadX(););
        GAMEPAD_PRESSED(GamepadButtonCode::PAD_BTN_Y, onPadY(););

        GAMEPAD_ROLL_LEFT(onGamepadRollLeft(roll););
        GAMEPAD_ROLL_RIGHT(onGamepadRollRight(roll););

        KEY_PRESSED(D1,
                    msaaEnabled ? Application::get().setSampleSize(1) : Application::get().setSampleSize(8);
                    msaaEnabled = !msaaEnabled;
        );

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
        mainCamera.onUpdate(dt);
        // notify all players to rotate a survival pack
        GDHeader header(CLIENT_TO_CLIENT, ROTATE_SURVIVAL_PACK);
        GDFloat rotation(0.005f);
        udp::Client::getRequestQueue().push(header, rotation);
        tcp::Client::getRequestQueue().push(header, rotation);

        auto& skyboxTransform = Application::get().activeScene->getSkybox().get<Transform3dComponent>()->modelMatrix;
        skyboxTransform.rotation[1] += 0.001f;
        skyboxTransform.apply();

        car.getTransform().rotation[1] += 0.01f;
        car.applyTransform();

        auto hoveredTransform = hoveredEntity.get<Transform3dComponent>();
        if (hoveredTransform) {
            hoveredTransform->modelMatrix.rotation[1] += 0.001f;
            hoveredTransform->modelMatrix.apply();
        }

        if (EventRegistry::mouseHold(ButtonLeft)) {
            RUNTIME_INFO("mouseHold: button left");
            mainCamera.applyMouseRotate();
        }

        if (EventRegistry::mouseHold(ButtonRight)) {
            RUNTIME_INFO("mouseHold: button right");
            mainCamera.applyMouseMove();
        }

        if (EventRegistry::mouseHold(ButtonMiddle)) {
            RUNTIME_INFO("mouseHold: button middle");
            mainCamera.applyMouseZoom();
        }
    }

    void TestLayer::onWindowResized(const uint32_t &width, const uint32_t &height) {
        mainCamera.setAspectRatio(width, height);
    }

    void TestLayer::onKeyPressed(event::KeyCode keyCode) {
    }

    void TestLayer::onKeyHold(event::KeyCode keyCode) {
    }

    void TestLayer::onKeyReleased(event::KeyCode keyCode) {
    }

    void TestLayer::onKeyTyped(event::KeyCode keyCode) {
    }

    void TestLayer::onPadA() {
        RUNTIME_INFO("Gamepad button A pressed!");
        mainCamera.move(DOWN);
        mainCamera.applyView();
    }

    void TestLayer::onPadB() {
        RUNTIME_INFO("Gamepad button B pressed!");
        mainCamera.move(RIGHT);
        mainCamera.applyView();
    }

    void TestLayer::onPadX() {
        RUNTIME_INFO("Gamepad button X pressed!");
        mainCamera.move(LEFT);
        mainCamera.applyView();
    }

    void TestLayer::onPadY() {
        RUNTIME_INFO("Gamepad button Y pressed!");
        mainCamera.move(UP);
        mainCamera.applyView();
    }

    void TestLayer::onGamepadRollLeft(const GamepadRoll& roll) {
        RUNTIME_INFO("onGamepadRollLeft: (x: {0}, y: {1}, trigger: {2})", roll.x, roll.y, roll.triggered);
        mainCamera.move({ roll.x, roll.y, 1 });
        mainCamera.applyView();
    }

    void TestLayer::onGamepadRollRight(const GamepadRoll& roll) {
        RUNTIME_INFO("onGamepadRollRight: (x: {0}, y: {1}, trigger: {2})", roll.x, roll.y, roll.triggered);
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
}