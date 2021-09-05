//
// Created by mecha on 28.08.2021.
//

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
            Application::onCreate();
            CLIENT_INFO("onCreate()");
            pushLayer(new TestLayer());

            pushLayout(new engine::DemoLayout());
            pushLayout(new TestLayout());
        }

        void onUpdate() override {
            Application::onUpdate();
            CLIENT_INFO("onUpdate()");
        }

        void onDestroy() override {
            Application::onDestroy();
            CLIENT_INFO("onDestroy()");
        }

        void onKeyPressed(engine::KeyCode keyCode) override {
            Application::onKeyPressed(keyCode);

            if (keyCode == engine::KeyCode::Escape) {
                engine::Application::onWindowClosed();
            }
        }

    };

}