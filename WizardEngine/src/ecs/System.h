//
// Created by mecha on 27.09.2021.
//

#pragma once

#include "Scene.h"

namespace engine {

    class System {

    public:
        virtual ~System() = default;

    public:
        virtual void onUpdate() = 0;

    public:
        inline void setActiveScene(const Ref<Scene> &activeScene) {
            this->activeScene = activeScene;
        }

    protected:
        Ref<Scene> activeScene = nullptr;

    };

}