//
// Created by mecha on 04.01.2022.
//

#pragma once

#include <scripting/ScriptSystem.h>
#include <profiler/Profiler.h>

namespace engine::scripting {

    Ref<Scene> ScriptSystem::activeScene = nullptr;

    void ScriptSystem::onCreate() {
        PROFILE_FUNCTION();
        activeScene->getRegistry().each<NativeScript>([](NativeScript* sc) {
            sc->onCreateFunction();
        });
        activeScene->getRegistry().each<CppScript>([](CppScript* script) {
            script->scriptable->onCreate();
        });
    }

    void ScriptSystem::onUpdate(time::Time dt) {
        PROFILE_FUNCTION();
        activeScene->getRegistry().each<NativeScript>([&dt](NativeScript* sc) {
            sc->onUpdateFunction(dt);
        });
        activeScene->getRegistry().each<CppScript>([&dt](CppScript* script) {
            script->scriptable->onUpdate(dt);
        });
    }

    void ScriptSystem::onDestroy() {
        PROFILE_FUNCTION();
        if (!activeScene)
            return;

        activeScene->getRegistry().each<NativeScript>([](NativeScript* sc) {
            sc->onDestroyFunction();
        });
        activeScene->getRegistry().each<CppScript>([](CppScript* script) {
            script->scriptable->onDestroy();
            delete script->scriptable;
        });
    }
}
