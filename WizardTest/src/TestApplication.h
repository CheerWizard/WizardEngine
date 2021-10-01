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

    public:
        TestApplication(const engine::EngineType &engineType = engine::ENGINE_3D) : Application(engineType) {}

    protected:

        void onCreate() override {
            CLIENT_INFO("create()");
            Application::onCreate();

            pushLayer(new TestLayer());

            auto demoLayout = new engine::DemoLayout();
            demoLayout->showKeyPressed = engine::KeyCode::D1;
            pushLayout(demoLayout);

            pushLayout(new TestLayout());
            // create window for Scene.
            pushLayout(new engine::ScenePanel(activeScene, getWindowWidth(), getWindowHeight()));

            CLIENT_INFO("Current workspace '{0}'", CURRENT_WORKING_DIR);

            auto shape2dShaderProps = engine::ShaderProps {
                "shape2d",
                "v_shape2d",
                "f_shape2d"
            };
            loadShader(shape2dShaderProps);

            auto transform = engine::TransformComponent3d {
                "transform",
                { -1, -1, 10 },
                {90, 0, 0},
                {0.5, 0.5, 0.5}
            };
            transform.applyChanges();

            auto objData = loadObj("deagle");

            auto shape = engine::ShapeComponent {
                objData.vertexData,
                objData.indexData
            };

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
            entity.addComponent<engine::TransformComponent3d>(transform);
            entity.addComponent<engine::TextureComponent>(texture);

            //todo add FrameBuffer and set viewport for scene window to dock space!
            //todo fix 3D viewProjection3d. object is going to wide during rotation.
            //todo search and add different Material components.
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
            cameraController->setPosition({0, 0, -1});
            cameraController->applyChanges();
        }

        void onUpdate() override {
            Application::onUpdate();
            CLIENT_INFO("onUpdate()");

            if (entity.hasComponent<engine::TransformComponent3d>()) {
                auto& transform = entity.getComponent<engine::TransformComponent3d>().transformMatrix;

                transform.rotation.y += 0.001f;
                transform.applyChanges();
            }
        }

        void onKeyPressed(engine::KeyCode keyCode) override {
            Application::onKeyPressed(keyCode);

            if (keyCode == engine::KeyCode::D2) {
                engine::ImGuiLayer::toggleDockSpace();
            }
        }

        void onDestroy() override {
            Application::onDestroy();
            CLIENT_INFO("onDestroy()");
        }

    private:
        engine::Entity entity;

    };

}