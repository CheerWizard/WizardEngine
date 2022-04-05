//
// Created by mecha on 07.10.2021.
//

#include <gui/MeshLayout.h>

#include "imgui.h"

namespace engine::gui {

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

    void MeshLayout::onKeyPressed(event::KeyCode keyCode) {
        if (_isFocused) {
            _cameraController->onKeyPressed(keyCode);
        }
    }

    void MeshLayout::onKeyHold(event::KeyCode keyCode) {
        if (_isFocused) {
            _cameraController->onKeyHold(keyCode);
        }
    }

    void MeshLayout::onKeyReleased(event::KeyCode keyCode) {
        if (_isFocused) {
            _cameraController->onKeyReleased(keyCode);
        }
    }

    void MeshLayout::onKeyTyped(event::KeyCode keyCode) {
        if (_isFocused) {
            _cameraController->onKeyTyped(keyCode);
        }
    }

    void MeshLayout::rotateEntity(const time::Time &dt) {
        auto& transform = _entity.get<graphics::Transform3dComponent>();
        transform.rotation.y += 0.000025f / dt;
        updateModel3d(transform);
    }

    void MeshLayout::onRender(const time::Time &dt) {
        id = _frame->getColorAttachment(0).id;

        _cameraController->setDeltaTime(dt);

        if (_shouldRotateEntity) {
            rotateEntity(dt);
        }

        _frame->bind();
        // enables transparency
        graphics::setBlendMode(true);
        graphics::setBlendFunction(SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
        // write to stencil buffer
        graphics::setClearColor({0.2, 0.2, 0.2, 1});
        graphics::setDepthTest(true);
        graphics::setStencilTest(true);
        graphics::setStencilTestActions({ KEEP, KEEP, REPLACE });
        graphics::clearStencilBuffer();
        graphics::setStencilTestOperator(ALWAYS, 1, false);
        graphics::stencilMask(false);

        _renderer->render<Transform3dComponent, io::ModelVertex>(_entity);

        // stop write to stencil buffer
        graphics::setStencilTestOperator(NOT_EQUAL, 1, false);
        graphics::stencilMask(true);
        graphics::setDepthTest(false);

        graphics::FrameBuffer::bindDefault();
        graphics::clearColorBuffer();

        if (ImGui::Button("Auto-Rotate", { 120, 36 })) {
            setRotateEntity(!_shouldRotateEntity);
        }
    }
}