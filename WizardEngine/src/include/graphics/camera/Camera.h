//
// Created by mecha on 29.10.2021.
//

#pragma once

#include <ecs/Entity.h>
#include "CameraComponents.h"

namespace engine {

    class Camera2D : public Entity {

    public:
        Camera2D() = default;

        Camera2D(const std::string &tag, EntityContainer* container) : Entity(tag, container) {
            create();
        }

        Camera2D(EntityContainer* container) : Entity("Camera", container) {
            create();
        }

        ~Camera2D() = default;

    private:
        void create();

    };

    class Camera3D : public Entity {

    public:
        Camera3D() = default;

        Camera3D(const std::string &tag, const float& aspectRatio, EntityContainer* container) : Entity(tag, container) {
            create(aspectRatio);
        }

        Camera3D(const float& aspectRatio, EntityContainer* container) : Entity("Camera", container) {
            create(aspectRatio);
        }

        ~Camera3D() = default;

    private:
        void create(const float& aspectRatio);

    };

}