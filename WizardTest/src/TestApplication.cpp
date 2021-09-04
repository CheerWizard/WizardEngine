//
// Created by mecha on 28.08.2021.
//

#include "../../WizardEngine/src/Engine.h"

#include "TestLayer.h"

namespace test {

    class TestApplication : public engine::Application {

    protected:

        void onCreate() override {
            Application::onCreate();
            CLIENT_INFO("onCreate()");
            pushOverlay(new engine::ImGuiLayer());
            pushLayer(new TestLayer());
        }

        void onUpdate() override {
            Application::onUpdate();
            CLIENT_INFO("onUpdate()");
        }

        void onDestroy() override {
            Application::onDestroy();
            CLIENT_INFO("onDestroy()");
        }

    };

}

engine::Application* engine::createApplication() {
    CLIENT_INFO("Creating TestApplication...");
    return new test::TestApplication();
}
