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

            auto shape2dVertex = new engine::Vertex {
                {
                    engine::Attribute {
                        "position",
                        0,
                        engine::VEC3},
//                        engine::Attribute {
//                        "color",
//                        1,
//                        engine::VEC4}
                        }
            };

            auto shape2dShader = loadShader(shape2dShaderProps, shape2dVertex);
            shape2dShader->setUniform(engine::ViewProjectionMatrix {
                "viewProjection",
                engine::PerspectiveMatrix {
                    "perspective",
                    getAspectRatio()
                }
            });

            // load a 2D triangle.

            shape2dVertex->setCount(3); // 2D triangle requires 3 Vertex to draw.

            const uint32_t verticesSize = 9;
            float vertices[verticesSize] = {
                    -0.5f, -0.5f, 0.5f,
                    0.5f, -0.5f, 0.5f,
                    0.0f,  0.5f, 0.5f,
                    };

            const uint32_t indicesSize = 3;
            uint32_t indices[indicesSize] = {0, 1, 2};

            auto shape2dObject = CREATE_GRAPHICS_OBJECT(shape2dShader->getName());
            shape2dObject->verticesSize = verticesSize;
            shape2dObject->indicesSize = indicesSize;
            shape2dObject->vertices = vertices;
            shape2dObject->indices = indices;

            loadObject(shape2dObject);

            // TODO render system is not displaying anything!
            // WARN not attribute, shader, uniform, draw call issues!
        }

        void onUpdate() override {
            Application::onUpdate();
            CLIENT_INFO("onUpdate()");
        }

        void onDestroy() override {
            Application::onDestroy();
            CLIENT_INFO("destroy()");
        }

        void onKeyPressed(engine::KeyCode keyCode) override {
            Application::onKeyPressed(keyCode);

            if (keyCode == engine::KeyCode::Escape) {
                engine::Application::onWindowClosed();
            }
        }
    };

}