//
// Created by mecha on 11.04.2022.
//

#include <core/test_layer.h>

#define ROTATE_SURVIVAL_PACK 123

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

        graphics::MeshSource<BatchVertex<Vertex3d>>::getMesh("assets/model/DesertEagle.fbx", {
            [this](const BaseMeshComponent<BatchVertex<Vertex3d>>& mesh) {
                survivalBackPack.add<BaseMeshComponent<BatchVertex<Vertex3d>>>(mesh);
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
        TCP_CLIENT_CONNECT(localhost, 54000);

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

        GAMEPAD_PRESSED(GamepadButtonCode::PAD_BTN_A, onPadA(););
        GAMEPAD_PRESSED(GamepadButtonCode::PAD_BTN_B, onPadB(););
        GAMEPAD_PRESSED(GamepadButtonCode::PAD_BTN_X, onPadX(););
        GAMEPAD_PRESSED(GamepadButtonCode::PAD_BTN_Y, onPadY(););

        GAMEPAD_ROLL_LEFT(onGamepadRollLeft(roll););
        GAMEPAD_ROLL_RIGHT(onGamepadRollRight(roll););

        KEY_PRESSED(KeyCode::D1, audio::MediaPlayer::pause(););
        KEY_PRESSED(KeyCode::D2, audio::MediaPlayer::stop(););
        KEY_PRESSED(KeyCode::D3, audio::MediaPlayer::playStream(););
    }

    void TestLayer::onUpdate(time::Time deltaTime) {
        cameraController->setDeltaTime(deltaTime);
//        UDP_CLIENT_SEND("Hello from UDP!");
        // rotate SurvivalPack on every machine connected to a TCP server!
        // todo fix a crash and connection timeout here!
        GDHeader header(CLIENT_TO_CLIENT, ROTATE_SURVIVAL_PACK);
        GDPrimitive<f32> rotation(0.005f);
        TCP_GDP_REQUEST(header, rotation);
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
        RUNTIME_INFO("Gamepad button A pressed!");
        cameraController->applyMove(MoveType::DOWN);
    }

    void TestLayer::onPadB() {
        RUNTIME_INFO("Gamepad button B pressed!");
        cameraController->applyMove(MoveType::RIGHT);
    }

    void TestLayer::onPadX() {
        RUNTIME_INFO("Gamepad button X pressed!");
        cameraController->applyMove(MoveType::LEFT);
    }

    void TestLayer::onPadY() {
        RUNTIME_INFO("Gamepad button Y pressed!");
        cameraController->applyMove(MoveType::UP);
    }

    void TestLayer::onGamepadRollLeft(const GamepadRoll& roll) {
        RUNTIME_INFO("onGamepadRollLeft: (x: {0}, y: {1}, trigger: {2})", roll.x, roll.y, roll.triggered);
        cameraController->move({ roll.x, roll.y });
        cameraController->applyChanges();
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

    void TestLayer::tcp_socketNotCreated() {

    }

    void TestLayer::tcp_socketClosed() {

    }

    void TestLayer::tcp_connectionFailed() {

    }

    void TestLayer::tcp_connectionSucceeded() {

    }

    void TestLayer::udp_socketNotCreated() {

    }

    void TestLayer::udp_sendDataFailed(const std::string &data) {

    }

    void TestLayer::udp_socketClosed() {

    }

    void TestLayer::onGameDataReceived(const std::pair<YAML::Node, GDHeader> &gdNodeHeader) {
        RUNTIME_INFO("onGameDataReceived()");
        YAML::Node gdNode = gdNodeHeader.first;
        GDHeader header = gdNodeHeader.second;
        RUNTIME_INFO("GDHeader[address: {0}, type: {1}]", header.address, header.type);
        // dispatch header type
        if (header.type == ROTATE_SURVIVAL_PACK) {
            // unpack data
            GDPrimitive<f32> gdPrimitive;
            gdPrimitive.deserialize(gdNode);
            auto rotationY = gdPrimitive.value;
            RUNTIME_INFO("Rotating SurvivalPack with value: {0}", rotationY);
            // handle data
            auto& modelMatrix = survivalBackPack.get<Transform3dComponent>()->modelMatrix;
            modelMatrix.rotation.y += rotationY;
            updateModel3d(modelMatrix);
        }
    }
}