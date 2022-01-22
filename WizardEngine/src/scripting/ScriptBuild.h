//
// Created by mecha on 22.01.2022.
//

#pragma once

#include "ScriptComponents.h"
#include "../core/Build.h"

namespace engine {

    struct ScriptBuildProps {
        std::string engineLoggerName;
        std::string runtimeLoggerName;
    };

    class ScriptBuild final {

    public:
        static void init(const ScriptBuildProps& scriptBuildProps);
        template<typename T>
        static void addNativeScript(Entity& entity);
        static void addDLLScript(Entity& entity);

    private:
        static std::string dllName;
        static std::string dllPath;

    };

    template<typename T>
    void ScriptBuild::addNativeScript(Entity& entity) {
        entity.add<NativeScript>();
        entity.get<NativeScript>().bind<T>();
    }

    template<typename T>
    inline void addNativeScript(Entity& entity) {
        ScriptBuild::addNativeScript<T>();
    }

    inline void addDLLScript(Entity& entity) {
        ScriptBuild::addDLLScript(entity);
    }

}
