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

        BIND_GAMEPAD_BUTTON_PRESSED(GamepadButtonCode::PAD_BTN_A, ENGINE_INFO("Button A pressed!"););
        BIND_GAMEPAD_BUTTON_PRESSED(GamepadButtonCode::PAD_BTN_B, ENGINE_INFO("Button B pressed!"););
        BIND_GAMEPAD_BUTTON_PRESSED(GamepadButtonCode::PAD_BTN_X, ENGINE_INFO("Button X pressed!"););
        BIND_GAMEPAD_BUTTON_PRESSED(GamepadButtonCode::PAD_BTN_Y, ENGINE_INFO("Button Y pressed!"););

        BIND_KEY_PRESSED(KeyCode::D1, survivalBackPack.update<CullingComponent>(true););
        BIND_KEY_PRESSED(KeyCode::D2, survivalBackPack.update<CullingComponent>(true););
        BIND_KEY_PRESSED(KeyCode::D3, survivalBackPack.update<CullingComponent>(false););
    }

    void TestLayer::onUpdate(time::Time deltaTime) {
        cameraController->setDeltaTime(deltaTime);
        // rotate survival backpack
        auto& modelMatrix = survivalBackPack.get<Transform3dComponent>()->modelMatrix;
        modelMatrix.rotation.y += 0.005f;
        updateModel3d(modelMatrix);
    }

    void TestLayer::onWindowResized(const uint32_t &width, const uint32_t &height) {
        Layer::onWindowResized(width, height);
        cameraController->onWindowResized(width, height);
    }

    void TestLayer::onKeyPressed(event::KeyCode keyCode) {
        Layer::onKeyPressed(keyCode);
        cameraController->onKeyPressed(keyCode);
    }

    void TestLayer::onKeyHold(event::KeyCode keyCode) {
        Layer::onKeyHold(keyCode);
        cameraController->onKeyHold(keyCode);
    }

    void TestLayer::onKeyReleased(event::KeyCode keyCode) {
        Layer::onKeyReleased(keyCode);
        cameraController->onKeyReleased(keyCode);
    }

    void TestLayer::onKeyTyped(event::KeyCode keyCode) {
        Layer::onKeyTyped(keyCode);
        cameraController->onKeyTyped(keyCode);
    }
}