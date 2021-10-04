//
// Created by mecha on 04.10.2021.
//

#include "FLApplication.h"

engine::Application* engine::createApplication() {
    CLIENT_INFO("Creating FLApplication...");
    return new fl::FLApplication();
}

int main() {
    engine::run();
}

namespace fl {

    void FLLayer::onUpdate(engine::Time deltaTime) {
        ImGuiLayer::onUpdate(deltaTime);
    }

    void FLLayer::onCreate() {
        Layer::onCreate();
    }

    void FLLayer::onDestroy() {
        ImGuiLayer::onDestroy();
    }

    void FLApplication::onCreate() {
        Application::onCreate();
        CLIENT_INFO("Current workspace '{0}'", CURRENT_WORKING_DIR);

        auto* flLayer = new FLLayer(
getNativeWindow(),
      getWindowWidth(),
      getWindowHeight()
        );

        flLayer->pushLayout(new engine::SceneLayout(activeScene, {
            "Scene",
            getWindowWidth(),
            getWindowHeight()
        }));

        pushOverlay(flLayer);

        auto shape2dShaderProps = engine::ShaderProps {
            "shape2d",
            "v_shape2d",
            "f_shape2d"
        };
        loadShader(shape2dShaderProps);

        loadTexture("demo.png");
        auto carObj = loadObj("human");

        // create car entity

        auto transform = engine::TransformComponent3d {
            "transform",
            { 2.5, 0, 12 },
            {135, 0, 0},
            {0.5, 0.5, 0.5}
        };
        transform.applyChanges();

        carObj.applyChanges();

        auto carTexture = engine::TextureComponent {
            "diffuseSampler",
            0
        };
        carTexture.applyChanges();

        carEntity = activeScene->createEntity("Car");
        carEntity.addComponent<engine::TransformComponent3d>(transform);
        carEntity.addComponent<engine::ShapeComponent>(carObj);
        carEntity.addComponent<engine::TextureComponent>(carTexture);
    }

    void FLApplication::onPrepare() {
        Application::onPrepare();
        CLIENT_INFO("onPrepare()");
        closeKeyPressed = engine::KeyCode::Escape;
        cameraController->bind(engine::KeyCode::W, engine::MoveType::UP);
        cameraController->bind(engine::KeyCode::A, engine::MoveType::LEFT);
        cameraController->bind(engine::KeyCode::S, engine::MoveType::DOWN);
        cameraController->bind(engine::KeyCode::D, engine::MoveType::RIGHT);
        cameraController->bind(engine::KeyCode::Q, engine::RotateType::LEFT_Z);
        cameraController->bind(engine::KeyCode::E, engine::RotateType::RIGHT_Z);
        cameraController->bind(engine::KeyCode::Z, engine::ZoomType::IN);
        cameraController->bind(engine::KeyCode::X, engine::ZoomType::OUT);
        cameraController->setPosition({0, 0, -1});
        cameraController->applyChanges();
    }

    void FLApplication::onUpdate() {
        Application::onUpdate();

        auto dt = fpsTimer.getDeltaTime();

        if (carEntity.hasComponent<engine::TransformComponent3d>()) {
            auto& transform = carEntity.getComponent<engine::TransformComponent3d>().transformMatrix;

            transform.rotation.y += 0.0001f / dt;
            transform.applyChanges();
        }
    }

    void FLApplication::onKeyPressed(engine::KeyCode keyCode) {
        Application::onKeyPressed(keyCode);

        if (keyCode == engine::KeyCode::D1) {
            setPolygonMode(engine::PolygonMode::POINT);
        }

        if (keyCode == engine::KeyCode::D2) {
            setPolygonMode(engine::PolygonMode::LINE);
        }

        if (keyCode == engine::KeyCode::D3) {
            setPolygonMode(engine::PolygonMode::FILL);
        }

        if (keyCode == engine::KeyCode::D0) {
            fpsTimer.setMaxFps(15);
        }

        if (keyCode == engine::KeyCode::D9) {
            fpsTimer.setMaxFps(30);
        }

        if (keyCode == engine::KeyCode::D8) {
            fpsTimer.setMaxFps(60);
        }

        if (keyCode == engine::KeyCode::D7) {
            fpsTimer.setMaxFps(getRefreshRate());
        }
    }

    void FLApplication::onDestroy() {
        Application::onDestroy();
        CLIENT_INFO("onDestroy()");
    }

}