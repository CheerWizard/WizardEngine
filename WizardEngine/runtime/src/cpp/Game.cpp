//
// Created by mecha on 04.10.2021.
//

#include <Game.h>
#include <Launcher.h>

engine::Application* engine::createApplication() {
    return new game::Application();
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
    engine::WindowProps Application::createWindowProps() {
        return engine::WindowProps { "Game", 800, 600 };
    }
}