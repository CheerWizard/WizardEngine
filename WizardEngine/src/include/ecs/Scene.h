//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <ecs/Entity.h>
#include <graphics/camera/Camera.h>

namespace engine::ecs {

    class ENGINE_API Scene : public EntityContainer {

    public:
        Scene() {
            name = "Untitled";
        }

        Scene(const std::string& name) : name(name) {}

        ~Scene();

    public:
        inline void setSkybox(const Entity& skybox) {
            this->skybox = skybox;
        }

        [[nodiscard]] inline const Entity& getSkybox() const {
            return skybox;
        }

        inline void setCamera(const graphics::Camera3D& camera) {
            this->camera = camera;
        }

        [[nodiscard]] inline graphics::Camera3D& getCamera() {
            return camera;
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
        graphics::Camera3D camera;
    };

}