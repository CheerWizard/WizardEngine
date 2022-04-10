//
// Created by mecha on 29.10.2021.
//

#pragma once

#include <ecs/Entity.h>
#include <graphics/camera/CameraComponents.h>

namespace engine::graphics {

    class Camera2D : public ecs::Entity {

    public:
        Camera2D() = default;

        Camera2D(const std::string &tag, ecs::EntityContainer* container) : ecs::Entity(tag, container) {
            create();
        }

        Camera2D(ecs::EntityContainer* container) : ecs::Entity("Camera", container) {
            create();
        }

        ~Camera2D() = default;

    private:
        void create();

    };

    class Camera3D : public ecs::Entity {

    public:
        Camera3D() = default;

        Camera3D(const std::string &tag, const float& aspectRatio, ecs::EntityContainer* container) : ecs::Entity(tag, container) {
            create(aspectRatio);
        }

        Camera3D(const float& aspectRatio, ecs::EntityContainer* container) : ecs::Entity("Camera", container) {
            create(aspectRatio);
        }

        ~Camera3D() = default;

    private:
        void create(const float& aspectRatio);

    };

}