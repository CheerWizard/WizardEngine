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

namespace test {

    class TestLayer : public Layer {

    public:
        TestLayer() : Layer() {
            init();
        }
        ~TestLayer() override = default;

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

    private:
        Ref<Camera3dController> cameraController;
        Entity survivalBackPack;
    };

}
