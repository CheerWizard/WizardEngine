//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <ecs/Entity.h>

namespace engine::ecs {

    class Scene : public EntityContainer {

    public:
        Scene(const char* name = "Untitled") : name(name) {}
        ~Scene();

    public:
        inline void setSkybox(const Entity& skybox) {
            this->skybox = skybox;
        }

        [[nodiscard]] inline const Entity& getSkybox() const {
            return skybox;
        }

        inline const std::string& getName() {
            return name;
        }

        inline void setName(const std::string& name) {
            this->name = name;
        }

    public:
        Entity findEntity(const uuid& uuid);
        Entity findEntity(const UUIDComponent& uuid);
        Entity findEntity(int uuid);

    private:
        std::string name;
        Entity skybox;
    };

}