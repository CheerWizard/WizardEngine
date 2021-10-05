//
// Created by mecha on 04.10.2021.
//

#pragma once

#include "core/Logger.h"

// needs to be defined on Client side.
extern engine::Application* engine::createApplication();

int main() {
    INIT_CLIENT_LOG("Client");
    INIT_ENGINE_LOG("Engine");

    ENGINE_INFO("Running Application...");
    auto* app = engine::createApplication();
    app->run();

    ENGINE_INFO("Application terminated!");
    delete app;
}

