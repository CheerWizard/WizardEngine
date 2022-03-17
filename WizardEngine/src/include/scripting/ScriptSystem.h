//
// Created by mecha on 04.01.2022.
//

#pragma once

#include <ecs/Scene.h>
#include "ScriptComponents.h"

namespace engine {

    class ScriptSystem {

    public:
        ScriptSystem() = default;
        ~ScriptSystem() = default;

    public:
        void onUpdate(Time dt);
        void onDestroy();

    private:
        void update(entt::registry& registry, Time dt);
        void destroy(entt::registry& registry);

    public:
        inline void setActiveScene(const Ref<Scene> &activeScene) {
            this->activeScene = activeScene;
        }

    private:
        Ref<Scene> activeScene = nullptr;

    };

}

