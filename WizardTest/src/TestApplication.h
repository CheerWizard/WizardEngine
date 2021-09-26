//
// Created by mecha on 28.08.2021.
//

#pragma once

#include "../../WizardEngine/src/Engine.h"

#include "TestLayer.h"

namespace test {

    class TestLayout : public engine::Layout {

    public:
        TestLayout() = default;
        ~TestLayout() override = default;

    public:
        void onUpdate(engine::Time deltaTime) override {
            ImGui::Begin("Test");
            ImGui::Text("Hello World!");
            ImGui::End();
        }

    };

    class TestApplication : public engine::Application {

    protected:

        void onCreate() override {
            CLIENT_INFO("create()");
            Application::onCreate();

            pushLayer(new TestLayer());

            pushLayout(new engine::DemoLayout());
            pushLayout(new TestLayout());

            CLIENT_INFO("Current workspace '{0}'", CURRENT_WORKING_DIR);

            auto shape2dShaderProps = engine::ShaderProps {
                "shape2d",
                "v_shape2d",
                "f_shape2d"
            };
            shape2dShaderName = shape2dShaderProps.name;

            auto shape2dVertex = new engine::VertexFormat {
                {
                    engine::Attribute {
                        "position",
                        0,
                        engine::VEC3},
                        engine::Attribute {
                        "textureCoords",
                        1,
                        engine::VEC2
                        },
                        engine::Attribute {
                        "color",
                        2,
                        engine::VEC4}
                }
            };

            auto shape2dShader = loadShader(shape2dShaderProps, shape2dVertex);

            auto transform = new engine::TransformMatrix {
                "transform",
                {0.5, 0.5, -0.5},
                {0, 0, 0},
                {1, 1, 1}
            };
            transform->applyChanges();

            auto brightness = new engine::FloatUniform {
                "brightness",
                1.0f
            };
            brightness->applyChanges();

            // load a 2D triangle.
            auto triangle = CREATE_CUBE(shape2dShaderName);
            shape2dIndex = addObject(triangle);
            triangle->isUpdated = true;
            triangle->brightness = brightness;
            triangle->transform = transform;

            enableDepthRendering();

            loadTexture("demo.png");

            auto textureSampler = new engine::TextureSampler {
                "diffuseSampler",
                0
            };

            triangle->textureSampler = textureSampler;

            //todo fix 3D viewProjection3d. object is going to wide during rotation.
            //todo add ECS - important as it's a core for all future systems.
            //todo Add Material system.
        }

        void onPrepare() override {
            Application::onPrepare();
            CLIENT_INFO("onPrepare()");
            closeKeyPressed = engine::KeyCode::Escape;
            cameraController->bind(engine::KeyCode::W, engine::MoveType::UP);
            cameraController->bind(engine::KeyCode::A, engine::MoveType::LEFT);
            cameraController->bind(engine::KeyCode::S, engine::MoveType::DOWN);
            cameraController->bind(engine::KeyCode::D, engine::MoveType::RIGHT);
            cameraController->bind(engine::KeyCode::Q, engine::RotateType::LEFT_Z);
            cameraController->bind(engine::KeyCode::E, engine::RotateType::RIGHT_Z);
            cameraController->bind(engine::KeyCode::Z, engine::ZoomType::IN);
            cameraController->bind(engine::KeyCode::X, engine::ZoomType::OUT);
            cameraController->moveSpeed = 0.1f;
            cameraController->zoomSpeed = 0.1f;
            cameraController->rotateSpeed = 0.1f;
            cameraController->applyChanges();
        }

        void onUpdate() override {
            Application::onUpdate();
            CLIENT_INFO("onUpdate()");

            auto object = getGraphicsObject(shape2dShaderName, shape2dIndex);
            auto transform = object->transform;

//            transform->rotation.z += 0.001f;
//            transform->rotation.x += 0.001f;
//            transform->rotation.y += 0.001f;
//            transform->applyChanges();
        }

        void onDestroy() override {
            Application::onDestroy();
            CLIENT_INFO("onDestroy()");
        }

    private:
        std::string shape2dShaderName;
        uint32_t shape2dIndex;

    };

}