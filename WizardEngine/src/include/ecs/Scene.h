//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <ecs/Entity.h>

namespace engine::ecs {

    class Scene : public EntityContainer {

    public:
        Scene(const char* name = "Untitled") : name(name) {}

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

        inline const char* getName() {
            return name;
        }

        inline void setName(const char* name) {
            this->name = name;
        }

    private:
        const char* name;
        Entity skybox;
    };

}