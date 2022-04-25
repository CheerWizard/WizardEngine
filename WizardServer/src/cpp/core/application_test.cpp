//
// Created by mecha on 05.04.2022.
//

#include <core/application_test.h>
#include <core/test_layer.h>

Application* core::createApplication() {
    RUNTIME_INFO("Creating Test Application...");
    return new test::Application();
}

extern const char* engineLogName() {
    return "Engine";
}

extern const char* editorLogName() {
    return "Editor";
}

extern const char* runtimeLogName() {
    return "Test";
}

namespace test {

    void Application::onCreate() {
        core::Application::onCreate();
        core::Application::pushFront(new TestLayer());
    }

    WindowProps Application::createWindowProps() {
        return WindowProps {
            "Test"
        };
    }
}