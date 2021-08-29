//
// Created by mecha on 28.08.2021.
//
#include "TestApplication.h"

TestApplication::TestApplication() = default;
TestApplication::~TestApplication() = default;

engine::Application* engine::createApplication() {
    return new TestApplication();
}
