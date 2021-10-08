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
            setTextureId(_renderer->getFrameColors()[0]);

            if (_cameraController != nullptr) {
                _cameraController->setDeltaTime(dt);
            }

            if (_meshComponent != nullptr && _transformComponent != nullptr) {
                auto& transform = _transformComponent->transformMatrix;

                transform.rotation.y += 0.0001f / dt;
                transform.applyChanges();

                _renderer->renderMesh(*_meshComponent, *_transformComponent);
            }
        }
        ImageLayout::onUpdate(dt);
    }

    void MeshLayout::destroy() {
        removeCallback();
    }

    void MeshLayout::onImageResized(const uint32_t &width, const uint32_t &height) {
        if (width == 0 || height == 0) return;

        _renderer->resizeFrame(width, height);
        if (_cameraController != nullptr) {
            _cameraController->onWindowResized(width, height);
        }
    }

    void MeshLayout::setCameraController(const Ref<Camera3dController> &camera3DController) {
        _cameraController = camera3DController;
        _renderer->setCameraController(_cameraController);
    }

    void MeshLayout::onKeyPressed(KeyCode keyCode) {
        if (_cameraController != nullptr && _isFocused) {
            _cameraController->onKeyPressed(keyCode);
        }
    }

    void MeshLayout::onKeyHold(KeyCode keyCode) {
        if (_cameraController != nullptr && _isFocused) {
            _cameraController->onKeyHold(keyCode);
        }
    }

    void MeshLayout::onKeyReleased(KeyCode keyCode) {
        if (_cameraController != nullptr && _isFocused) {
            _cameraController->onKeyReleased(keyCode);
        }
    }

    void MeshLayout::onKeyTyped(KeyCode keyCode) {
        if (_cameraController != nullptr && _isFocused) {
            _cameraController->onKeyTyped(keyCode);
        }
    }

}