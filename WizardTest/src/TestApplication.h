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
            pushLayout(new engine::SceneLayout(activeScene, {
                "Scene Preview",
                        600,
                        480
            }));
            pushLayout(new engine::SceneLayout(activeScene, {
                "Scene",
                getWindowWidth(),
                getWindowHeight()
            }));

            CLIENT_INFO("Current workspace '{0}'", CURRENT_WORKING_DIR);

            auto shape2dShaderProps = engine::ShaderProps {
                "shape2d",
                "v_shape2d",
                "f_shape2d"
            };
            loadShader(shape2dShaderProps);

            loadTexture("demo.png");

            auto objData = loadObj("deagle");

            // create deagle entity

            auto deagle = engine::ShapeComponent {
                objData.vertexData,
                objData.indexData
            };
            deagle.applyChanges();

            auto deagleTransform = engine::TransformComponent3d {
                "transform",
                { 2.5, 0, 12 },
                {0, 0, 0},
                {0.5, 0.5, 0.5}
            };
            deagleTransform.applyChanges();

            auto deagleTexture = engine::TextureComponent {
                "diffuseSampler",
                0
            };
            deagleTexture.applyChanges();

            deagleEntity = activeScene->createEntity("Deagle");
            deagleEntity.addComponent<engine::ShapeComponent>(deagle);
            deagleEntity.addComponent<engine::TransformComponent3d>(deagleTransform);
            deagleEntity.addComponent<engine::TextureComponent>(deagleTexture);

            // create cube entity

            auto cube = engine::Cube();

            cube.vertexData.vertexStart += deagle.vertexData.vertexCount;
            cube.indexData.indexStart += deagle.indexData.indexCount;

            for (auto i = 0 ; i < cube.vertexData.vertexCount ; i++) {
                auto& vertex = cube.vertexData.vertices[i];
                vertex.position.x -= 5;
            }
            cube.applyChanges();

            auto cubeTransform = engine::TransformComponent3d {
                "transform",
                { 2.5, 0, 12 },
                {0, 0, 0},
                {0.5, 0.5, 0.5}
            };
            cubeTransform.applyChanges();

            auto cubeTexture = engine::TextureComponent {
                "diffuseSampler",
                0
            };
            cubeTexture.applyChanges();

            cubeEntity = activeScene->createEntity("Cube");
            cubeEntity.addComponent<engine::ShapeComponent>(cube);
            cubeEntity.addComponent<engine::TransformComponent3d>(cubeTransform);
            cubeEntity.addComponent<engine::TextureComponent>(cubeTexture);

            // create triangle entity

            auto triangle = engine::Triangle();

            triangle.vertexData.vertexStart += deagle.vertexData.vertexCount + cube.vertexData.vertexCount;
            triangle.indexData.indexStart += deagle.indexData.indexCount + cube.indexData.indexCount;

            for (auto i = 0 ; i < triangle.vertexData.vertexCount ; i++) {
                auto& vertex = triangle.vertexData.vertices[i];
                vertex.position.x -= 10;
            }
            triangle.applyChanges();

            auto triangleTransform = engine::TransformComponent3d {
                "transform",
                { -2.5, 0, 12 },
                {0, 0, 0},
                {0.5, 0.5, 0.5}
            };
            triangleTransform.applyChanges();

            auto triangleTexture = engine::TextureComponent {
                "diffuseSampler",
                0
            };
            triangleTexture.applyChanges();

            triangleEntity = activeScene->createEntity("Triangle");
            triangleEntity.addComponent<engine::ShapeComponent>(triangle);
            triangleEntity.addComponent<engine::TransformComponent3d>(triangleTransform);
            triangleEntity.addComponent<engine::TextureComponent>(triangleTexture);
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

//            if (entity.hasComponent<engine::TransformComponent3d>()) {
//                auto& transform = entity.getComponent<engine::TransformComponent3d>().transformMatrix;
//
//                transform.rotation.y += 0.001f;
//                transform.applyChanges();
//            }
//
//            if (entity2.hasComponent<engine::TransformComponent3d>()) {
//                auto& transform = entity2.getComponent<engine::TransformComponent3d>().transformMatrix;
//
//                transform.rotation.y -= 0.001f;
//                transform.applyChanges();
//            }
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
        engine::Entity cubeEntity;
        engine::Entity triangleEntity;
        engine::Entity deagleEntity;

    };

}