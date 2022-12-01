//
// Created by mecha on 11.04.2022.
//

#pragma once

#include <core/Application.h>
#include <graphics/GraphicsObject.h>
#include <graphics/camera/Camera.h>
#include <graphics/light/Light.h>

namespace test {

    using namespace engine;
    using namespace engine::graphics;

    class TestLayer : public Layer,
            tcp::ClientListener, tcp::SenderListener, tcp::ReceiverListener,
            udp::ClientListener, udp::SenderListener, udp::ReceiverListener {

    public:
        TestLayer() : Layer() {
            init();
        }
        ~TestLayer() override;

    public:
        void onPrepare() override;
        void onUpdate(Time deltaTime) override;

        void onWindowResized(const uint32_t &width, const uint32_t &height) override;

        void onKeyPressed(KeyCode keyCode) override;
        void onKeyHold(KeyCode keyCode) override;
        void onKeyReleased(KeyCode keyCode) override;
        void onKeyTyped(KeyCode keyCode) override;

    public:
        void onTCPSocketCreated() override;
        void onTCPSocketClosed() override;

        void onTCPConnectionFailed() override;
        void onTCPConnected() override;

        void onTCPSenderFailed(char *data, size_t size) override;
        void onTCPSenderSuccess() override;

        void onTCPReceiverFailed(char *data, size_t size) override;
        void onTCPReceiverSuccess(const YAML::Node &gdNode, const GDHeader &header) override;

        void onUDPSocketCreated() override;
        void onUDPSocketClosed() override;

        void onUDPSenderFailed(char *data, size_t size) override;
        void onUDPSenderSuccess() override;

        void onUDPReceiverFailed(char *data, size_t size) override;
        void onUDPReceiverSuccess(const YAML::Node &gdNode, const GDHeader &header) override;

    public:
        void onWindowClosed() override;
        void onMousePressed(MouseCode mouseCode) override;
        void onMouseRelease(MouseCode mouseCode) override;
        void onMouseScrolled(double xOffset, double yOffset) override;
        void onCursorMoved(double xPos, double yPos) override;

        void onVisualDraw(time::Time dt) override;

    private:
        void bindCamera();
        void init();
        void onPadA();
        void onPadB();
        void onPadX();
        void onPadY();

        void dragLight();

        void switchMSAA();
        void switchHDR();
        void switchBlur();
        void switchSharpen();
        void switchEdgeDetection();
        void switchGaussianBlur();
        void switchBloom();

        void onGamepadRollLeft(const GamepadRoll& roll);
        void onGamepadRollRight(const GamepadRoll& roll);

    private:
        Camera3D mainCamera;
        vector<Batch3d> packs;
        vector<Instance3d> instancedPacks;
        bool msaaEnabled = false;
        PhongLight light;
        bool showGizmo = false;
        ImageLayout sceneViewport {
            "Scene",
                    512,
                    512
        };
    };

}
