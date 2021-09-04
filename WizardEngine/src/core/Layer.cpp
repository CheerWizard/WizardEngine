//
// Created by mecha on 31.08.2021.
//

#include "Layer.h"

void engine::Layer::onCreate() {
    ENGINE_INFO("{0} : onCreate()", tag);
}

void engine::Layer::onUpdate(engine::Time deltaTime) {
    ENGINE_INFO("{0} : onUpdate()", tag);
}

void engine::Layer::onDestroy() {
    ENGINE_INFO("{0} : onDestroy()", tag);
}

void engine::Layer::onImGuiRender() {
    ENGINE_INFO("{0} : onImGuiRender()", tag);
}

void engine::Layer::onWindowClosed() {
    ENGINE_INFO("{0} : onWindowClosed()", tag);
}

void engine::Layer::onWindowResized(unsigned int width, unsigned int height) {
    ENGINE_INFO("{0} : onWindowResized({1}, {2})", tag, width, height);
}

void engine::Layer::onKeyPressed(engine::KeyCode keyCode) {
    ENGINE_INFO("{0} : onKeyPressed({1})", tag, keyCode);
}

void engine::Layer::onKeyHold(engine::KeyCode keyCode) {
    ENGINE_INFO("{0} : onKeyHold({1})", tag, keyCode);
}

void engine::Layer::onKeyReleased(engine::KeyCode keyCode) {
    ENGINE_INFO("{0} : onKeyReleased({1})", tag, keyCode);
}

void engine::Layer::onMousePressed(engine::MouseCode mouseCode) {
    ENGINE_INFO("{0} : onMousePressed({1})", tag, mouseCode);
}

void engine::Layer::onMouseRelease(engine::MouseCode mouseCode) {
    ENGINE_INFO("{0} : onMouseReleased({1})", tag, mouseCode);
}

void engine::Layer::onMouseScrolled(double xOffset, double yOffset) {
    ENGINE_INFO("{0} : onMouseScrolled({1}, {2})", tag, xOffset, yOffset);
}

void engine::Layer::onCursorMoved(double xPos, double yPos) {
    ENGINE_INFO("{0} : onCursorMoved({1}, {2})", tag, xPos, yPos);
}

void engine::Layer::onKeyTyped(engine::KeyCode keyCode) {
    ENGINE_INFO("{0} : onKeyTyped({1})", tag, keyCode);
}
