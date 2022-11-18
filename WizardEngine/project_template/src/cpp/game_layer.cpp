//
// Created by mecha on 11.04.2022.
//

#include <game_layer.h>
#include "../include/game_layer.h"

namespace game {

    game::GameLayer::GameLayer() {
        engine::scripting::ScriptManager::init();
    }

    GameLayer::~GameLayer() {
        engine::scripting::ScriptManager::free();
    }

    void GameLayer::onPrepare() {
    }

    void GameLayer::onUpdate(Time deltaTime) {
    }

    void GameLayer::onWindowResized(const uint32_t &width, const uint32_t &height) {
    }

    void GameLayer::onKeyPressed(KeyCode keyCode) {
    }

    void GameLayer::onKeyHold(KeyCode keyCode) {
    }

    void GameLayer::onKeyReleased(KeyCode keyCode) {
    }

    void GameLayer::onKeyTyped(KeyCode keyCode) {
    }

    void GameLayer::onWindowClosed() {
    }

    void GameLayer::onMousePressed(MouseCode mouseCode) {
    }

    void GameLayer::onMouseRelease(MouseCode mouseCode) {
    }

    void GameLayer::onMouseScrolled(double xOffset, double yOffset) {
    }

    void GameLayer::onCursorMoved(double xPos, double yPos) {
    }

    void GameLayer::onVisualDraw(Time dt) {
    }
}