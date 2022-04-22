//
// Created by mecha on 04.01.2022.
//

#pragma once

#include <scripting/ScriptSystem.h>

namespace engine::scripting {

    void ScriptSystem::onUpdate(Time dt) {
        auto& registry = activeScene->getRegistry();

        registry.each<NativeScript>([&dt, this](NativeScript* sc) {
            if (!sc->script) {
                sc->script = { activeScene.get(), sc->entityId };
                sc->onCreateFunction(sc->script);
            }
            sc->onUpdateFunction(sc->script, dt);
        });

        registry.each<DLLScript>([&dt](DLLScript* sc) {
            if (sc->scriptable) {
                sc->scriptable->onUpdate(dt);
            }
        });
    }

    void ScriptSystem::onDestroy() {
        auto& registry = activeScene->getRegistry();

        registry.each<NativeScript>([](NativeScript* sc) {
            if (sc->script) {
                sc->onDestroyFunction(sc->script);
            }
        });

        registry.each<DLLScript>([](DLLScript* sc) {
            if (sc->scriptable) {
                sc->scriptable->onDestroy();
                delete sc->scriptable;
                sc->scriptable = nullptr;
            }
        });
    }
}
