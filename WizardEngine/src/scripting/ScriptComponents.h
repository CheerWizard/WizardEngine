//
// Created by mecha on 04.01.2022.
//

#pragma once

#include "../ecs/Entity.h"
#include "../core/Time.h"

namespace engine {

    struct NativeScriptComponent {
        Entity* script = nullptr;
        // lifecycle functions
        std::function<void()> destructFunction;
        std::function<void(Entity*)> onCreateFunction;
        std::function<void(Entity*)> onDestroyFunction;
        std::function<void(Entity*, Time)> onUpdateFunction;

        template<class T>
        void bind() {
            destructFunction = [&]() { delete (T*)script; };
            onCreateFunction = [](Entity* script) { ((T*) script)->onCreate(); };
            onDestroyFunction = [](Entity* script) { ((T*) script)->onDestroy(); };
            onUpdateFunction = [](Entity* script, Time dt) { ((T*) script)->onUpdate(dt); };
        }
    };

    template<typename T>
    void addScript(Entity& entity) {
        entity.add<NativeScriptComponent>();
        entity.get<NativeScriptComponent>().bind<T>();
    }

}
