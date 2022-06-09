//
// Created by mecha on 05.04.2022.
//

#include <game_application.h>
#include <game_layer.h>

Application* engine::core::createApplication() {
    RUNTIME_INFO("Creating Test Application...");
    return new test::Application();
}

extern const char* engineLogName() {
    return "Engine";
}

extern const char* editorLogName() {
    return "Editor";
}

extern const char* runtimeLogName() {
    return "Game";
}

namespace game {

    void Application::onCreate() {
        engine::core::Application::onCreate();
        engine::core::Application::pushFront(new GameLayer());
    }

    WindowProps Application::createWindowProps() {
        return WindowProps {
                "Test"
        };
    }
}