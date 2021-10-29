//
// Created by mecha on 07.10.2021.
//

#include "MeshLayout.h"

namespace engine {

    void MeshLayout::create() {
        setCallback(this);
    }

    void MeshLayout::onUpdate(Time dt) {
        if (_isVisible) {
            setTextureId(_frameController->getFrameColors()[0]);

            _cameraController->setDeltaTime(dt);

            auto& transform = _entity.get<Transform3dComponent>();
            transform.rotation.y += 0.0001f / dt;
            transform.applyChanges();

            _frameController->begin();
            _renderer->render(_entity);
            _frameController->end();
        }

        ImageLayout::onUpdate(dt);
    }

    void MeshLayout::destroy() {
        removeCallback();
    }

    void MeshLayout::onImageResized(const uint32_t &width, const uint32_t &height) {
        if (width == 0 || height == 0) return;

        _frameController->resize(width, height);
        _cameraController->onWindowResized(width, height);
    }

    void MeshLayout::onKeyPressed(KeyCode keyCode) {
        if (_isFocused) {
            _cameraController->onKeyPressed(keyCode);
        }
    }

    void MeshLayout::onKeyHold(KeyCode keyCode) {
        if (_isFocused) {
            _cameraController->onKeyHold(keyCode);
        }
    }

    void MeshLayout::onKeyReleased(KeyCode keyCode) {
        if (_isFocused) {
            _cameraController->onKeyReleased(keyCode);
        }
    }

    void MeshLayout::onKeyTyped(KeyCode keyCode) {
        if (_isFocused) {
            _cameraController->onKeyTyped(keyCode);
        }
    }
}