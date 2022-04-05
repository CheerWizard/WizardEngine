//
// Created by mecha on 05.04.2022.
//

#include <core/application_test.h>
#include <graphics/core/geometry/Point.h>

using namespace engine::core;
using namespace engine::graphics;

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
        createTest();
    }

    WindowProps Application::createWindowProps() {
        return WindowProps {
            "Test"
        };
    }

    void Application::createTest() {
        auto scene1 = createRef<Scene>();
        setActiveScene(scene1);

        auto points = Entity("Points", scene1.get());
        points.add<Points>(new PointVertex[4] {
                { { -0.5, 0.5 }, { 1, 0, 0 } },
                { { 0.5, 0.5 }, { 0, 1, 0 } },
                { { 0.5, -0.5 }, { 0, 0, 1 }},
                { { -0.5, -0.5 }, { 1, 1, 0 }}
        }, 4);
    }
}