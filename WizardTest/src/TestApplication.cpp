//
// Created by mecha on 28.08.2021.
//
#include "TestApplication.h"

engine::Application* engine::createApplication() {
    return new TestApplication();
}

void TestApplication::onCreate() {
    engine::Logger::getClientLogger()->info("Test App has been created!");
//    CLIENT_INFO("Test App has been created!");
}
