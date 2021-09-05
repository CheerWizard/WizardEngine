//
// Created by mecha on 04.09.2021.
//

#include "TestApplication.h"

engine::Application* engine::createApplication() {
    CLIENT_INFO("Creating TestApplication...");
    return new test::TestApplication();
}

int main() {
    engine::run();
}