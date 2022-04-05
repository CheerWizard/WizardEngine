//
// Created by mecha on 22.01.2022.
//

#include <scripting/ScriptBuilder.h>

namespace engine::scripting {

    void ScriptBuilder::init(const ScriptBuildProps &scriptBuildProps) {
        thread::VoidTask<const ScriptBuildProps&> task = {
                "ScriptBuildInit_Task",
                "ScriptBuildInit_Thread",
                initTask
        };
        task.run(scriptBuildProps);
    }

    void ScriptBuilder::initTask(const ScriptBuildProps &scriptBuildProps) {
        if (!filesystem::exists(SCRIPT_LIB_PATH)) {
            rebuild();
        }
        build::Libs::add("ScriptDLL", SCRIPT_LIB_PATH);
        build::createFunction("ScriptDLL", "initLogs", scriptBuildProps.engineLoggerName, scriptBuildProps.runtimeLoggerName);
    }

    void ScriptBuilder::rebuildTask() {
        thread::VoidTask<> task = {
                "ScriptRebuild_Task",
                "ScriptRebuild_Thread",
                rebuild
        };
        task.run();
    }

    void ScriptBuilder::rebuild() {
        if (!filesystem::exists(SCRIPT_PROJECT_PATH)) {
            SCRIPT_PROJECT_GENERATE;
        }
        if (!filesystem::exists("../ScriptDLL/WizardEngine.lib")) {
            filesystem::copy(ENGINE_LIB_PATH, "../ScriptDLL/WizardEngine.lib");
        }
        SCRIPT_PROJECT_BUILD;
    }

    void ScriptBuilder::addDLLScript(Entity &entity, const std::string &scriptName) {
        auto dllScript = build::createObject<Scriptable>("ScriptDLL", "create" + scriptName);
        if (dllScript) {
            dllScript->entity = entity;
            dllScript->onCreate();
            entity.add<DLLScript>(dllScript);
        }
    }

}


