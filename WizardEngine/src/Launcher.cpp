//
// Created by mecha on 29.08.2021.
//

#include "core/Application.h"
#include "core/Logger.h"

using namespace engine;

extern Application* engine::createApplication();

int main() {
    Logger::createClientLogger("Client");
    Logger::createEngineLogger("Engine");
    Logger::setPattern("%^[%T] %n: %v%$");

    auto app = createApplication();
    app->run();
    delete app;
}