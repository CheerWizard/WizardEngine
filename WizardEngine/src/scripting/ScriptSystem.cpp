//
// Created by mecha on 04.01.2022.
//

#pragma once

#include "ScriptSystem.h"

namespace engine {

    void ScriptSystem::onUpdate(Time dt) {
        activeScene->getRegistry().view<NativeScript>().each([=](auto entity, auto& sc) {
           if (!sc.script) {
               sc.script = { entity, activeScene.get() };
               sc.onCreateFunction(sc.script);
           }
            sc.onUpdateFunction(sc.script, dt);
        });
        activeScene->getRegistry().view<DLLScript>().each([=](auto entity, auto& sc) {
            if (sc.scriptable) {
                sc.scriptable->onUpdate(dt);
            }
        });
    }

    void ScriptSystem::onDestroy() {
        activeScene->getRegistry().view<NativeScript>().each([=](auto entity, auto& sc) {
            if (sc.script) {
                sc.onDestroyFunction(sc.script);
            }
        });
        activeScene->getRegistry().view<DLLScript>().each([=](auto entity, auto& sc) {
            if (sc.scriptable) {
                sc.scriptable->onDestroy();
                delete sc.scriptable;
                sc.scriptable = nullptr;
            }
        });
    }

}
