//
// Created by mecha on 04.01.2022.
//

#pragma once

#include <scripting/ScriptSystem.h>
#include <profiler/Profiler.h>

namespace engine::scripting {

    void ScriptSystem::onUpdate(time::Time dt) {
        PROFILE_FUNCTION();
        auto& registry = activeScene->getRegistry();

        registry.each<NativeScript>([&dt, this](NativeScript* sc) {
            if (!sc->script) {
                sc->script = { activeScene.get(), sc->entityId };
                sc->onCreateFunction(sc->script);
            }
            sc->onUpdateFunction(sc->script, dt);
        });
    }

    void ScriptSystem::onDestroy() {
        PROFILE_FUNCTION();
        auto& registry = activeScene->getRegistry();

        registry.each<NativeScript>([](NativeScript* sc) {
            if (sc->script) {
                sc->onDestroyFunction(sc->script);
            }
        });
    }
}
