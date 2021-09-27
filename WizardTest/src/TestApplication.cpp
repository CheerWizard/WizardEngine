//
// Created by mecha on 05.09.2021.
//

#include "TestApplication.h"

engine::Application* engine::createApplication() {
    CLIENT_INFO("Creating TestApplication...");
    return new test::TestApplication();
}

int main() {
    engine::run();
}