//
// Created by mecha on 04.01.2022.
//

#pragma once

#include <scripting/Scriptable.h>

namespace engine::scripting {

    component(NativeScript) {
        Entity script;
        // lifecycle functions
        std::function<void(Entity&)> onCreateFunction;
        std::function<void(Entity&)> onDestroyFunction;
        std::function<void(Entity&, time::Time)> onUpdateFunction;

        template<class T>
        void bind() {
            onCreateFunction = [](Entity& script) { ((T&) script).onCreate(); };
            onDestroyFunction = [](Entity& script) { ((T&) script).onDestroy(); };
            onUpdateFunction = [](Entity& script, time::Time dt) { ((T&) script).onUpdate(dt); };
        }
    };

    component(DLLScript) {
        Scriptable* scriptable = nullptr;
    };

}
