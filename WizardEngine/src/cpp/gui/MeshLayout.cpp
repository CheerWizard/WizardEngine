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
        _camera3D->setAspectRatio(width, height);
    }

    void MeshLayout::onKeyPressed(event::KeyCode keyCode) {
        if (_isFocused) {
//            _camera3D->onKeyPressed(keyCode);
        }
    }

    void MeshLayout::onKeyHold(event::KeyCode keyCode) {
        if (_isFocused) {
//            _camera3D->onKeyHold(keyCode);
        }
    }

    void MeshLayout::onKeyReleased(event::KeyCode keyCode) {
        if (_isFocused) {
//            _camera3D->onKeyReleased(keyCode);
        }
    }

    void MeshLayout::onKeyTyped(event::KeyCode keyCode) {
        if (_isFocused) {
//            _camera3D->onKeyTyped(keyCode);
        }
    }

    void MeshLayout::rotateEntity(const time::Time &dt) {
        auto& transform = *_entity.get<graphics::Transform3dComponent>();
        transform.modelMatrix.rotation.v[1] += 0.000025f / dt;
        math::updateModel3d(transform.modelMatrix);
    }

    void MeshLayout::onRender(const time::Time &dt) {
        ColorAttachment texture;
        if (_frame->getColorAttachment(0, texture)) {
            id = texture.id;
        }

        _camera3D->onUpdate(dt);

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

        _renderer->render(_entity);

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