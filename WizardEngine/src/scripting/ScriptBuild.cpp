//
// Created by mecha on 22.01.2022.
//

#include "ScriptBuild.h"

namespace engine {

    std::string ScriptBuild::dllName = "ScriptDLL";
    std::string ScriptBuild::dllPath = "../ScriptDLL/ScriptDLL.dll";

    void ScriptBuild::init(const ScriptBuildProps &scriptBuildProps) {
        engine::Libs::add(dllName, dllPath);
        engine::createFunction(dllName, "initLogs", scriptBuildProps.engineLoggerName, scriptBuildProps.runtimeLoggerName);
    }

    void ScriptBuild::addDLLScript(Entity &entity) {
        auto dllScript = engine::createObject<Scriptable>(dllName, "create");
        if (dllScript) {
            dllScript->entity = entity;
            dllScript->onCreate();
            entity.add<DLLScript>(dllScript);
        }
    }

}


