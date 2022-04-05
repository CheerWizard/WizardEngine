//
// Created by mecha on 31.08.2021.
//

#include <core/Layer.h>
#include <io/Logger.h>

namespace engine::core {

    void Layer::onWindowClosed() {
        ENGINE_INFO("{0} : onWindowClosed()", tag);
    }

    void Layer::onWindowResized(const uint32_t &width, const uint32_t &height) {
        ENGINE_INFO("{0} : onWindowResized({1}, {2})", tag, width, height);
    }

    void Layer::onKeyPressed(event::KeyCode keyCode) {
        ENGINE_INFO("{0} : onKeyPressed({1})", tag, keyCode);
    }

    void Layer::onKeyHold(event::KeyCode keyCode) {
        ENGINE_INFO("{0} : onKeyHold({1})", tag, keyCode);
    }

    void Layer::onKeyReleased(event::KeyCode keyCode) {
        ENGINE_INFO("{0} : onKeyReleased({1})", tag, keyCode);
    }

    void Layer::onMousePressed(event::MouseCode mouseCode) {
        ENGINE_INFO("{0} : onMousePressed({1})", tag, mouseCode);
    }

    void Layer::onMouseRelease(event::MouseCode mouseCode) {
        ENGINE_INFO("{0} : onMouseReleased({1})", tag, mouseCode);
    }

    void Layer::onMouseScrolled(double xOffset, double yOffset) {
        ENGINE_INFO("{0} : onMouseScrolled({1}, {2})", tag, xOffset, yOffset);
    }

    void Layer::onCursorMoved(double xPos, double yPos) {
        ENGINE_INFO("{0} : onCursorMoved({1}, {2})", tag, xPos, yPos);
    }

    void Layer::onKeyTyped(event::KeyCode keyCode) {
        ENGINE_INFO("{0} : onKeyTyped({1})", tag, keyCode);
    }

}