//
// Created by mecha on 04.10.2021.
//

#include "FLLayer.h"

namespace fairy {

    void FLLayer::create() {
        _scenePanel = new engine::ImageLayout({
            "Scene",
            props.width,
            props.height
        });
        _entityPanel = new engine::EntityLayout({
            "Entities"
        });

        auto shape2dShaderProps = engine::ShaderProps {
            "shape2d",
            "v_shape2d",
            "f_shape2d"
        };

        app->loadShader(shape2dShaderProps);
        app->loadTexture("demo.png");
        auto carObj = app->loadObj("human");

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

        carEntity = app->activeScene->createEntity("Car");
        carEntity.addComponent<engine::TransformComponent3d>(transform);
        carEntity.addComponent<engine::ShapeComponent>(carObj);
        carEntity.addComponent<engine::TextureComponent>(carTexture);
    }

    void FLLayer::destroy() {
        delete _scenePanel;
        delete _entityPanel;
    }

    void FLLayer::onRender(engine::Time dt) {
        _scenePanel->onUpdate(dt);
        _entityPanel->onUpdate(dt);
    }

    void FLLayer::onPrepare() {
        CLIENT_INFO("onPrepare()");
        ImGuiLayer::onPrepare();
        _scenePanel->setTextureId(app->activeScene->getTextureId());
        app->closeKeyPressed = engine::KeyCode::Escape;
        app->cameraController->bind(engine::KeyCode::W, engine::MoveType::UP);
        app->cameraController->bind(engine::KeyCode::A, engine::MoveType::LEFT);
        app->cameraController->bind(engine::KeyCode::S, engine::MoveType::DOWN);
        app->cameraController->bind(engine::KeyCode::D, engine::MoveType::RIGHT);
        app->cameraController->bind(engine::KeyCode::Q, engine::RotateType::LEFT_Z);
        app->cameraController->bind(engine::KeyCode::E, engine::RotateType::RIGHT_Z);
        app->cameraController->bind(engine::KeyCode::Z, engine::ZoomType::IN);
        app->cameraController->bind(engine::KeyCode::X, engine::ZoomType::OUT);
        app->cameraController->setPosition({0, 0, -1});
        app->cameraController->applyChanges();
    }

    void FLLayer::onUpdate(engine::Time dt) {
        ImGuiLayer::onUpdate(dt);

        if (carEntity.hasComponent<engine::TransformComponent3d>()) {
            auto& transform = carEntity.getComponent<engine::TransformComponent3d>().transformMatrix;

            transform.rotation.y += 0.0001f / dt;
            transform.applyChanges();
        }
    }

    void FLLayer::onKeyPressed(engine::KeyCode keyCode) {
        Layer::onKeyPressed(keyCode);

        if (keyCode == engine::KeyCode::D1) {
            app->setPolygonMode(engine::PolygonMode::POINT);
        }

        if (keyCode == engine::KeyCode::D2) {
            app->setPolygonMode(engine::PolygonMode::LINE);
        }

        if (keyCode == engine::KeyCode::D3) {
            app->setPolygonMode(engine::PolygonMode::FILL);
        }

        if (keyCode == engine::KeyCode::D0) {
            app->fpsTimer.setMaxFps(15);
        }

        if (keyCode == engine::KeyCode::D9) {
            app->fpsTimer.setMaxFps(30);
        }

        if (keyCode == engine::KeyCode::D8) {
            app->fpsTimer.setMaxFps(60);
        }

        if (keyCode == engine::KeyCode::D7) {
            app->fpsTimer.setMaxFps(app->getRefreshRate());
        }
    }

}