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

        _frame->resize(width, height);
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
        updateModel3d(transform);
    }

    void MeshLayout::onRender(const Time &dt) {
        id = _frame->getColorAttachments()[0];

        _cameraController->setDeltaTime(dt);

        if (_shouldRotateEntity) {
            rotateEntity(dt);
        }

        _frame->bind();
        // enables transparency
        setBlendMode(true);
        setBlendFunction(SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
        // write to stencil buffer
        setClearColor({0.2, 0.2, 0.2, 1});
        setDepthTest(true);
        setStencilTest(true);
        setStencilTestActions({ KEEP, KEEP, REPLACE });
        clearStencilBuffer();
        setStencilTestOperator(ALWAYS, 1, false);
        stencilMask(false);

        _renderer->render<Transform3dComponent, ObjVertex>(_entity);

        // stop write to stencil buffer
        setStencilTestOperator(NOT_EQUAL, 1, false);
        stencilMask(true);
        setDepthTest(false);

        _frame->unbind();
        clearColorBuffer();

        if (ImGui::Button("Auto-Rotate", { 120, 36 })) {
            setRotateEntity(!_shouldRotateEntity);
        }
    }
}