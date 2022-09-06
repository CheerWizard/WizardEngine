//
// Created by mecha on 04.10.2021.
//

#pragma once

#include <io/Logger.h>
#include <profiler/Profiler.h>

// needs to be defined on Client side.
extern engine::core::Application* engine::core::createApplication();
extern const char* engineLogName();
extern const char* editorLogName();
extern const char* runtimeLogName();

int main() {
    auto* engineLog = engineLogName();
    auto* editorLog = editorLogName();
    auto* runtimeLog = runtimeLogName();
    INIT_ENGINE_LOG(engineLog);
    INIT_EDITOR_LOG(editorLog);
    INIT_RUNTIME_LOG(runtimeLog);

    auto* app = engine::core::createApplication();
    PROFILE_SESSION_BEGIN("WizardProfile.json");
    app->run();
    PROFILE_SESSION_END();
    delete app;
}

