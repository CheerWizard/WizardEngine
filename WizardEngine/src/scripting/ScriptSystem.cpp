//
// Created by mecha on 04.01.2022.
//

#pragma once

#include "ScriptSystem.h"

namespace engine {

    void ScriptSystem::onUpdate(Time dt) {
        activeScene->getRegistry().view<StaticScript>().each([=](auto entity, auto& sc) {
           if (!sc.script) {
               sc.script = { entity, activeScene.get() };
               sc.onCreateFunction(sc.script);
           }
            sc.onUpdateFunction(sc.script, dt);
        });
        activeScene->getRegistry().view<DynamicScript>().each([=](auto entity, auto& sc) {
            if (!sc.script) {
                sc.script = new Entity();
                sc.script->onCreate();
            }
            sc.script->onUpdate(dt);
        });
    }

    void ScriptSystem::onDestroy() {
        activeScene->getRegistry().view<StaticScript>().each([=](auto entity, auto& sc) {
            if (sc.script) {
                sc.onDestroyFunction(sc.script);
            }
        });
        activeScene->getRegistry().view<DynamicScript>().each([=](auto entity, auto& sc) {
            if (sc.script) {
                sc.script->onDestroy();
                delete sc.script;
                sc.script = nullptr;
            }
        });
    }

}
