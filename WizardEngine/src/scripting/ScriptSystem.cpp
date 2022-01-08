//
// Created by mecha on 04.01.2022.
//

#pragma once

#include "ScriptSystem.h"

namespace engine {

    void ScriptSystem::onUpdate(Time dt) {
        activeScene->getRegistry().view<NativeScriptComponent>().each([=](entt::entity entity, auto& ns) {
           if (!ns.script) {
               ns.script = new Entity(entity, activeScene.get());
               ns.onCreateFunction(ns.script);
           }
           ns.onUpdateFunction(ns.script, dt);
        });
    }

    void ScriptSystem::onDestroy() {
        activeScene->getRegistry().view<NativeScriptComponent>().each([=](auto entity, auto& ns) {
            if (ns.script) {
                ns.onDestroyFunction(ns.script);
                ns.destructFunction();
            }
        });
    }

}
