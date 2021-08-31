//
// Created by mecha on 29.08.2021.
//

#include "core/Application.h"

using namespace engine;

extern Application* engine::createApplication();

int main() {
    INIT_CLIENT_LOG("Client");
    INIT_ENGINE_LOG("Engine");
    LOG_PATTERN("%^[%T] %n: %v%$");

    auto app = createApplication();
    app->run();
    delete app;
}