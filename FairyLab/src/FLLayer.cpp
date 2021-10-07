//
// Created by mecha on 04.10.2021.
//

#include "FLLayer.h"

namespace fairy {

    void FLLayer::create() {
        _scenePreviewCallback = new FLLayer::ScenePreviewCallback(*this);
        _imagePreviewCallback = new FLLayer::ImagePreviewCallback(*this);

        _scenePanel.setCallback(_scenePreviewCallback);
        _imagePreview.setCallback(_imagePreviewCallback);
        _entityPanel.setCallback(this);
        _assetBrowser->setCallback(this);

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

        _carEntity = app->activeScene->createEntity("Car");
        _carEntity.addComponent<engine::TransformComponent3d>(transform);
        _carEntity.addComponent<engine::MeshComponent>(carObj);
        _carEntity.addComponent<engine::TextureComponent>(carTexture);
    }

    void FLLayer::destroy() {
        _entityPanel.removeCallback();
        _scenePanel.removeCallback();
        _imagePreview.removeCallback();
        delete _imagePreviewCallback;
        delete _scenePreviewCallback;
    }

    void FLLayer::onPrepare() {
        CLIENT_INFO("onPrepare()");
        ImGuiLayer::onPrepare();
        app->closeKeyPressed = engine::KeyCode::Escape;
        app->sceneCameraController->bind(engine::KeyCode::W, engine::MoveType::UP);
        app->sceneCameraController->bind(engine::KeyCode::A, engine::MoveType::LEFT);
        app->sceneCameraController->bind(engine::KeyCode::S, engine::MoveType::DOWN);
        app->sceneCameraController->bind(engine::KeyCode::D, engine::MoveType::RIGHT);
        app->sceneCameraController->bind(engine::KeyCode::Q, engine::RotateType::LEFT_Z);
        app->sceneCameraController->bind(engine::KeyCode::E, engine::RotateType::RIGHT_Z);
        app->sceneCameraController->bind(engine::KeyCode::Z, engine::ZoomType::IN);
        app->sceneCameraController->bind(engine::KeyCode::X, engine::ZoomType::OUT);
        app->sceneCameraController->setPosition({0, 0, -1});
        app->sceneCameraController->applyChanges();

        _scenePanel.setTextureId(app->activeScene->getTextureId());

        _imagePreview.hide();
        _imagePreview.setClosable(true);
    }

    void FLLayer::onRender(engine::Time dt) {
        _scenePanel.onUpdate(dt);
        _entityPanel.onUpdate(dt);
        _assetBrowser->onUpdate(dt);
        _imagePreview.onUpdate(dt);
    }

    void FLLayer::onUpdate(engine::Time dt) {
        ImGuiLayer::onUpdate(dt);

        if (_carEntity.hasComponent<engine::TransformComponent3d>()) {
            auto& transform = _carEntity.getComponent<engine::TransformComponent3d>().transformMatrix;

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

    engine::Entity FLLayer::createEntity(const std::string &entityName) {
        return app->activeScene->createEntity(entityName);
    }

    void FLLayer::removeEntity(engine::Entity entity) {
        app->activeScene->deleteEntity(entity);
    }

    void FLLayer::onMousePressed(engine::MouseCode mouseCode) {
        _scenePanel.onMousePressed(mouseCode);
    }

    void FLLayer::onMouseRelease(engine::MouseCode mouseCode) {
        _scenePanel.onMouseRelease(mouseCode);
    }

    void FLLayer::onFileOpen(const std::string &fileName) {
        ENGINE_INFO("onImageFileOpen({0})", fileName);
        _imagePreview.load(fileName);
        _imagePreview.show();
    }

    void FLLayer::ImagePreviewCallback::onImageResized(const uint32_t &width, const uint32_t &height) {
        // do nothing.
    }

    void FLLayer::ScenePreviewCallback::onImageResized(const uint32_t &width, const uint32_t &height) {
        _parent.app->onWindowResized(width, height);
    }

}