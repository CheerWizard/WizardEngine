//
// Created by mecha on 04.01.2022.
//

#pragma once

#include <ecs/Scene.h>
#include <scripting/ScriptComponents.h>

namespace engine::scripting {

    class ScriptSystem {

    public:
        ScriptSystem() = default;
        ~ScriptSystem() = default;

    public:
        void onUpdate(time::Time dt);
        void onDestroy();

    public:
        inline void setActiveScene(const core::Ref<Scene> &activeScene) {
            this->activeScene = activeScene;
        }

    private:
        core::Ref<Scene> activeScene = nullptr;

    };

}

