//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <ecs/Entity.h>

namespace engine::ecs {

    class Scene : public EntityContainer {

    public:
        Scene() = default;

        ~Scene() {
            clear();
        }

    public:
        inline void setSkybox(const Entity& skybox) {
            this->skybox = skybox;
        }

        [[nodiscard]] inline const Entity& getSkybox() const {
            return skybox;
        }

    private:
        Entity skybox;
    };

}