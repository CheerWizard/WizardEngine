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
            // todo dont use for now. creates overhead for drivers, maybe even need to restart PC :)
            auto carObj = loadObj("ferrari");

            // create car entity

            auto transform = engine::TransformComponent3d {
                "transform",
                { 2.5, 0, 12 },
                {135, 0, 0},
                {0.5, 0.5, 0.5}
            };
            transform.applyChanges();

            carObj.applyChanges();

            auto carTexture = engine::TextureComponent {
                "diffuseSampler",
                0
            };
            carTexture.applyChanges();

            carEntity = activeScene->createEntity("Car");
            carEntity.addComponent<engine::TransformComponent3d>(transform);
            carEntity.addComponent<engine::ShapeComponent>(carObj);
            carEntity.addComponent<engine::TextureComponent>(carTexture);

//            auto deagleObj = loadObj("deagle");
//            auto ak47Obj = loadObj("Ak47");
//            auto humanObj = loadObj("human");
//
//            // create human entity
//
//            auto transform = engine::TransformComponent3d {
//                "transform",
//                { 2.5, 0, 12 },
//                {135, 0, 0},
//                {0.5, 0.5, 0.5}
//            };
//            transform.applyChanges();
//
//            for (auto i = 0 ; i < humanObj.vertexData.vertexCount ; i++) {
//                auto& vertex = humanObj.vertexData.vertices[i];
//                vertex.position.x -= 40;
//            }
//            humanObj.applyChanges();
//
//            auto humanTexture = engine::TextureComponent {
//                "diffuseSampler",
//                0
//            };
//            humanTexture.applyChanges();
//
//            humanEntity = activeScene->createEntity("Human");
//            humanEntity.addComponent<engine::TransformComponent3d>(transform);
//            humanEntity.addComponent<engine::ShapeComponent>(humanObj);
//            humanEntity.addComponent<engine::TextureComponent>(humanTexture);
//
//            // create AK47 entity
//            for (auto i = 0 ; i < ak47Obj.vertexData.vertexCount ; i++) {
//                auto& vertex = ak47Obj.vertexData.vertices[i];
//                vertex.position.x -= 30;
//                vertex.position.y -= 30;
//            }
//            ak47Obj.applyChanges();
//
//            auto ak47Texture = engine::TextureComponent {
//                "diffuseSampler",
//                0
//            };
//            ak47Texture.applyChanges();
//
//            ak47Entity = activeScene->createEntity("Human2");
//            ak47Entity.addComponent<engine::ShapeComponent>(ak47Obj);
//            ak47Entity.addComponent<engine::TextureComponent>(ak47Texture);
//
//            // create deagle entity
//
//            deagleObj.applyChanges();
//            for (auto i = 0 ; i < deagleObj.vertexData.vertexCount ; i++) {
//                auto& vertex = deagleObj.vertexData.vertices[i];
//                vertex.position.x -= 20;
//            }
//            deagleObj.applyChanges();
//
//            auto deagleTexture = engine::TextureComponent {
//                "diffuseSampler",
//                0
//            };
//            deagleTexture.applyChanges();
//
//            deagleEntity = activeScene->createEntity("Deagle");
//            deagleEntity.addComponent<engine::ShapeComponent>(deagleObj);
//            deagleEntity.addComponent<engine::TextureComponent>(deagleTexture);
//
//            // create cube entity
//
//            auto cube = engine::Cube();
//
//            for (auto i = 0 ; i < cube.vertexData.vertexCount ; i++) {
//                auto& vertex = cube.vertexData.vertices[i];
//                vertex.position.x -= 10;
//            }
//            cube.applyChanges();
//
//            auto cubeTexture = engine::TextureComponent {
//                "diffuseSampler",
//                0
//            };
//            cubeTexture.applyChanges();
//
//            cubeEntity = activeScene->createEntity("Cube");
//            cubeEntity.addComponent<engine::ShapeComponent>(cube);
//            cubeEntity.addComponent<engine::TextureComponent>(cubeTexture);
//
//            // create triangle entity
//
//            auto triangle = engine::Triangle();
//            triangle.applyChanges();
//
//            auto triangleTexture = engine::TextureComponent {
//                "diffuseSampler",
//                0
//            };
//            triangleTexture.applyChanges();
//
//            triangleEntity = activeScene->createEntity("Triangle");
//            triangleEntity.addComponent<engine::ShapeComponent>(triangle);
//            triangleEntity.addComponent<engine::TextureComponent>(triangleTexture);
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

            if (carEntity.hasComponent<engine::TransformComponent3d>()) {
                auto& transform = carEntity.getComponent<engine::TransformComponent3d>().transformMatrix;

                transform.rotation.y += 0.0005f;
                transform.applyChanges();
            }

//            if (entity2.hasComponent<engine::TransformComponent3d>()) {
//                auto& transform = entity2.getComponent<engine::TransformComponent3d>().transformMatrix;
//
//                transform.rotation.y -= 0.001f;
//                transform.applyChanges();
//            }
        }

        void onKeyPressed(engine::KeyCode keyCode) override {
            Application::onKeyPressed(keyCode);

            if (keyCode == engine::KeyCode::D1) {
                setPolygonMode(engine::PolygonMode::POINT);
            }

            if (keyCode == engine::KeyCode::D2) {
                setPolygonMode(engine::PolygonMode::LINE);
            }

            if (keyCode == engine::KeyCode::D3) {
                setPolygonMode(engine::PolygonMode::FILL);
            }
        }

        void onDestroy() override {
            Application::onDestroy();
            CLIENT_INFO("onDestroy()");
        }

    private:
        engine::Entity triangleEntity;
        engine::Entity cubeEntity;
        engine::Entity deagleEntity;
        engine::Entity ak47Entity;
        engine::Entity humanEntity;
        engine::Entity carEntity;

    };

}