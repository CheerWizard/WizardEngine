//
// Created by mecha on 11.04.2022.
//

#include <game_layer.h>

namespace game {

    GameLayer::~GameLayer() {
        tcp::Client::close();
        udp::Client::close();
    }

    void GameLayer::onPrepare() {
    }

    void GameLayer::onUpdate(time::Time deltaTime) {
    }

    void GameLayer::onWindowResized(const uint32_t &width, const uint32_t &height) {
    }

    void GameLayer::onKeyPressed(event::KeyCode keyCode) {
    }

    void GameLayer::onKeyHold(event::KeyCode keyCode) {
    }

    void GameLayer::onKeyReleased(event::KeyCode keyCode) {
    }

    void GameLayer::onKeyTyped(event::KeyCode keyCode) {
    }

    void GameLayer::onWindowClosed() {
    }

    void GameLayer::onMousePressed(event::MouseCode mouseCode) {
    }

    void GameLayer::onMouseRelease(event::MouseCode mouseCode) {
    }

    void GameLayer::onMouseScrolled(double xOffset, double yOffset) {
    }

    void GameLayer::onCursorMoved(double xPos, double yPos) {
    }

    void GameLayer::onTCPSocketCreated() {
    }

    void GameLayer::onTCPConnectionFailed() {
    }

    void GameLayer::onTCPConnected() {
    }

    void GameLayer::onTCPSocketClosed() {
    }

    void GameLayer::onTCPSenderFailed(char *data, size_t size) {
    }

    void GameLayer::onTCPSenderSuccess() {
    }

    void GameLayer::onTCPReceiverFailed(char *data, size_t size) {
    }

    void GameLayer::onTCPReceiverSuccess(const YAML::Node &gdNode, const GDHeader &header) {
    }

    void GameLayer::onUDPSocketCreated() {
    }

    void GameLayer::onUDPSocketClosed() {
    }

    void GameLayer::onUDPSenderFailed(char *data, size_t size) {
    }

    void GameLayer::onUDPSenderSuccess() {
    }

    void GameLayer::onUDPReceiverFailed(char *data, size_t size) {
    }

    void GameLayer::onUDPReceiverSuccess(const YAML::Node &gdNode, const GDHeader &header) {
    }
}