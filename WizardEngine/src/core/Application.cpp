//
// Created by mecha on 28.08.2021.
//

#include "Application.h"
#include "Logger.h"

engine::Application::Application() = default;
engine::Application::~Application() = default;

void engine::Application::run() {
    ENGINE_INFO("Trying to create client...")
    onCreate();
    ENGINE_INFO("Client has been created!")

    while (true) {
        // do nothing.
    }
}
