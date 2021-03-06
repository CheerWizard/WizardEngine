//
// Created by mecha on 04.10.2021.
//

#pragma once

#include <io/Logger.h>
#include <scripting/ScriptBuilder.h>

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
//  scripting::ScriptBuilder::init({engineLog, runtimeLog});

    ENGINE_INFO("Running Application...");
    auto* app = engine::core::createApplication();
    app->run();

    ENGINE_INFO("Application terminated!");
    delete app;
}

