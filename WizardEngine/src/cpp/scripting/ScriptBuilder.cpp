//
// Created by mecha on 22.01.2022.
//

#include <scripting/ScriptBuilder.h>

namespace engine {

    void ScriptBuilder::init(const ScriptBuildProps &scriptBuildProps) {
        VoidTask<const ScriptBuildProps&> task = {
                "ScriptBuildInit_Task",
                "ScriptBuildInit_Thread",
                initTask
        };
        task.run(scriptBuildProps);
    }

    void ScriptBuilder::initTask(const ScriptBuildProps &scriptBuildProps) {
        if (!FileSystem::exists(SCRIPT_LIB_PATH)) {
            rebuild();
        }
        engine::Libs::add("ScriptDLL", SCRIPT_LIB_PATH);
        engine::createFunction("ScriptDLL", "initLogs", scriptBuildProps.engineLoggerName, scriptBuildProps.runtimeLoggerName);
    }

    void ScriptBuilder::rebuildTask() {
        VoidTask<> task = {
                "ScriptRebuild_Task",
                "ScriptRebuild_Thread",
                rebuild
        };
        task.run();
    }

    void ScriptBuilder::rebuild() {
        if (!FileSystem::exists(SCRIPT_PROJECT_PATH)) {
            SCRIPT_PROJECT_GENERATE;
        }
        if (!FileSystem::exists("../ScriptDLL/WizardEngine.lib")) {
            FileSystem::copy(ENGINE_LIB_PATH, "../ScriptDLL/WizardEngine.lib");
        }
        SCRIPT_PROJECT_BUILD;
    }

    void ScriptBuilder::addDLLScript(Entity &entity, const std::string &scriptName) {
        auto dllScript = engine::createObject<Scriptable>("ScriptDLL", "create" + scriptName);
        if (dllScript) {
            dllScript->entity = entity;
            dllScript->onCreate();
            entity.add<DLLScript>(dllScript);
        }
    }

}


