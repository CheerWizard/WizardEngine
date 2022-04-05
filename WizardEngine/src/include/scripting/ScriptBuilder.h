//
// Created by mecha on 22.01.2022.
//

#pragma once

#include <scripting/ScriptComponents.h>

#include <tools/Tools.h>
#include <build/Build.h>
#include <core/filesystem.h>

#define ENGINE_LIB_PATH "../WizardEngine/WizardEngine.lib"
#define SCRIPT_MAKE_FILE_PATH "../ScriptDLL/CMakeLists.txt"

#ifdef WIN32
#define SCRIPT_LIB_PATH "../ScriptDLL/Debug/ScriptDLL.dll"
#define SCRIPT_PROJECT_PATH "../ScriptDLL/ScriptDLL.sln"
#define SCRIPT_PROJECT_BUILD engine::tools::msBuild(SCRIPT_PROJECT_PATH)
#define SCRIPT_PROJECT_GENERATE engine::tools::cmake(SCRIPT_MAKE_FILE_PATH)
#endif

namespace engine::scripting {

    struct ScriptBuildProps {
        std::string engineLoggerName;
        std::string runtimeLoggerName;
    };

    class ScriptBuilder final {

    public:
        static void init(const ScriptBuildProps& scriptBuildProps);
        template<typename T>
        static void addNativeScript(Entity& entity);
        static void addDLLScript(Entity& entity, const std::string &scriptName);
        static void rebuild();
        static void rebuildTask();

    private:
        static void initTask(const ScriptBuildProps& scriptBuildProps);

    };

    template<typename T>
    void ScriptBuilder::addNativeScript(Entity& entity) {
        entity.add<NativeScript>();
        entity.get<NativeScript>().bind<T>();
    }

    template<typename T>
    inline void addNativeScript(Entity& entity) {
        ScriptBuilder::addNativeScript<T>();
    }

    inline void addDLLScript(Entity& entity, const std::string& scriptName) {
        ScriptBuilder::addDLLScript(entity, scriptName);
    }

}
