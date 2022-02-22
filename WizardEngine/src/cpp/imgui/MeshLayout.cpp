//
// Created by mecha on 07.10.2021.
//

#include <imgui/MeshLayout.h>
#include <graphics/transform/TransformComponents.h>

#include "imgui.h"

namespace engine {

    void MeshLayout::create() {
        setCallback(this);
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

    void MeshLayout::rotateEntity(const Time &dt) {
        auto& transform = _entity.get<Transform3dComponent>();
        transform.rotation.y += 0.000025f / dt;
        ModelMatrices::update(transform);
    }

    void MeshLayout::onRender(const Time &dt) {
        id = _frameController->getFrameColors()[0];

        _cameraController->setDeltaTime(dt);

        if (_shouldRotateEntity) {
            rotateEntity(dt);
        }

        _frameController->begin();
        _renderer->render(_entity);
        _frameController->end();

        if (ImGui::Button("Auto-Rotate", { 120, 36 })) {
            setRotateEntity(!_shouldRotateEntity);
        }
    }
}