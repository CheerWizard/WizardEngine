//
// Created by mecha on 04.01.2022.
//

#pragma once

#include <scripting/ScriptSystem.h>

namespace engine::scripting {

    void ScriptSystem::onUpdate(Time dt) {
        auto& registry = activeScene->getRegistry();

        registry.view<NativeScript>().each([=](auto entity, auto& sc) {
            if (!sc.script) {
                sc.script = { entity, activeScene.get() };
                sc.onCreateFunction(sc.script);
            }
            sc.onUpdateFunction(sc.script, dt);
        });

        registry.view<DLLScript>().each([=](auto entity, auto& sc) {
            if (sc.scriptable) {
                sc.scriptable->onUpdate(dt);
            }
        });
    }

    void ScriptSystem::onDestroy() {
        auto& registry = activeScene->getRegistry();

        registry.view<NativeScript>().each([=](auto entity, auto& sc) {
            if (sc.script) {
                sc.onDestroyFunction(sc.script);
            }
        });

        registry.view<DLLScript>().each([=](auto entity, auto& sc) {
            if (sc.scriptable) {
                sc.scriptable->onDestroy();
                delete sc.scriptable;
                sc.scriptable = nullptr;
            }
        });
    }
}
