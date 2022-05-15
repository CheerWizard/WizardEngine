//
// Created by mecha on 11.04.2022.
//

#pragma once

#include <core/Application.h>
#include <core/Layer.h>
#include <graphics/core/geometry/Point.h>
#include <graphics/camera/Camera.h>
#include <graphics/skybox/Skybox.h>
#include <graphics/core/geometry/Quad.h>
#include <graphics/GraphicsObject.h>
#include <graphics/camera/CameraController.h>
#include <audio/audio_source.h>

using namespace engine::core;
using namespace engine::graphics;
using namespace engine::event;
using namespace engine::time;
using namespace engine::network;

namespace test {

    class TestLayer : public Layer,
            tcp::ClientListener, udp::ClientListener {

    public:
        TestLayer() : Layer() {
            init();
        }
        ~TestLayer() override;

    public:
        void onUpdate(Time deltaTime) override;
        void onWindowResized(const uint32_t &width, const uint32_t &height) override;
        void onKeyPressed(KeyCode keyCode) override;
        void onKeyHold(KeyCode keyCode) override;
        void onKeyReleased(KeyCode keyCode) override;
        void onKeyTyped(KeyCode keyCode) override;

        void onPrepare() override;

    private:
        void bindCamera();
        void init();
        void onPadA();
        void onPadB();
        void onPadX();
        void onPadY();

    public:
        void tcp_socketNotCreated() override;
        void tcp_connectionFailed() override;
        void tcp_socketClosed() override;

        void udp_socketNotCreated() override;
        void udp_sendDataFailed(const std::string &data) override;
        void udp_socketClosed() override;

        void onWindowClosed() override;

        void onMousePressed(event::MouseCode mouseCode) override;

        void onMouseRelease(event::MouseCode mouseCode) override;

        void onMouseScrolled(double xOffset, double yOffset) override;

        void onCursorMoved(double xPos, double yPos) override;

    private:
        Ref<Camera3dController> cameraController;
        Entity survivalBackPack;
    };

}
