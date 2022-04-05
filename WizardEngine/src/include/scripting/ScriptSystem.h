//
// Created by mecha on 04.01.2022.
//

#pragma once

#include <ecs/Scene.h>
#include <scripting/ScriptComponents.h>

namespace engine::scripting {

    using namespace core;

    class ScriptSystem {

    public:
        ScriptSystem() = default;
        ~ScriptSystem() = default;

    public:
        void onUpdate(time::Time dt);
        void onDestroy();

    public:
        inline void setActiveScene(const Ref<Scene> &activeScene) {
            this->activeScene = activeScene;
        }

    private:
        Ref<Scene> activeScene = nullptr;
    };

}

