//
// Created by mecha on 04.01.2022.
//

#pragma once

#include <scripting/ScriptSystem.h>

namespace engine {

    void ScriptSystem::onUpdate(Time dt) {
        update(activeScene->getBatchRegistry(), dt);
        update(activeScene->getInstanceRegistry(), dt);
    }

    void ScriptSystem::onDestroy() {
        destroy(activeScene->getBatchRegistry());
        destroy(activeScene->getInstanceRegistry());
    }

    void ScriptSystem::update(entt::registry &registry, Time dt) {
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

    void ScriptSystem::destroy(entt::registry &registry) {
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
