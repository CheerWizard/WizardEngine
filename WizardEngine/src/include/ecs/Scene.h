//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <ecs/Entity.h>

namespace engine {

    class Scene : public EntityContainer {

    public:
        Scene() = default;

        ~Scene() {
            clear();
        }

        Scene(const Scene& scene) = default;

    public:
        inline void setSkybox(const Entity& skybox) {
            this->skybox = skybox;
        }

        inline const Entity& getSkybox() const {
            return skybox;
        }

    private:
        Entity skybox;
    };

}