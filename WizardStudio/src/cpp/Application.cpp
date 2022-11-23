//
// Created by mecha on 04.10.2021.
//

#include <Application.h>
#include <Activity.h>
#include <Launcher.h>

#include <graphics/skybox/Skybox.h>
#include <graphics/core/geometry/Quad.h>
#include <graphics/GraphicsObject.h>

engine::core::Application* engine::core::createApplication() {
    EDITOR_INFO("Creating Application...");
    return new studio::Application();
}

extern const char* engineLogName() {
    return "Engine";
}

extern const char* editorLogName() {
    return "Editor";
}

extern const char* runtimeLogName() {
    return "Runtime";
}

namespace studio {

    void Application::onCreate() {
        engine::core::Application::onCreate();
        EDITOR_INFO("Current workspace '{0}'", CURRENT_WORKING_DIR);

        auto* activity = new Activity(
                this,
                ImGuiLayerProps {
                    getWindowWidth(),
                    getWindowHeight()
                }
        );

        pushBack(activity);
    }

}