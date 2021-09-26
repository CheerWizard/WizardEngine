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

            auto demoLayout = new engine::DemoLayout();
            demoLayout->showKeyPressed = engine::KeyCode::D1;
            pushLayout(demoLayout);

            pushLayout(new TestLayout());

            CLIENT_INFO("Current workspace '{0}'", CURRENT_WORKING_DIR);

            auto shape2dShaderProps = engine::ShaderProps {
                "shape2d",
                "v_shape2d",
                "f_shape2d"
            };

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

            auto transform = engine::TransformComponent {
                "transform",
                {0.5, 0.5, -0.5},
                {0, 0, 0},
                {1, 1, 1}
            };
            transform.applyChanges();

            auto shape = engine::Cube();
            shape.applyChanges();

            enableDepthRendering();

            loadTexture("demo.png");

            auto texture = engine::TextureComponent {
                "diffuseSampler",
                0
            };
            texture.applyChanges();

            entity = activeScene->createEntity("DemoEntity");
            entity.addComponent<engine::ShapeComponent>(shape);
            entity.addComponent<engine::TransformComponent>(transform);
            entity.addComponent<engine::TextureComponent>(texture);

            //todo fix 3D viewProjection3d. object is going to wide during rotation.
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
            cameraController->applyChanges();
        }

        void onUpdate() override {
            Application::onUpdate();
            CLIENT_INFO("onUpdate()");

            auto transform = entity.getComponent<engine::TransformComponent>().transformMatrix;
            // todo if update component here, value will not be changed!
            // todo however, if you update Component for ex. in Renderer, value will be changed everywhere!
            transform.rotation.z += 0.001f;
            transform.rotation.x += 0.001f;
            transform.rotation.y += 0.001f;
            transform.applyChanges();
        }

        void onDestroy() override {
            Application::onDestroy();
            CLIENT_INFO("onDestroy()");
        }

    private:
        engine::Entity entity;

    };

}