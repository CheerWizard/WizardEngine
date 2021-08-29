//
// Created by mecha on 29.08.2021.
//

#include "core/Application.h"

extern engine::Application* engine::createApplication();

int main() {
    auto app = engine::createApplication();
    app->run();
    delete app;
}