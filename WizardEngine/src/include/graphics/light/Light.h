//
// Created by mecha on 04.11.2021.
//

#pragma once

#include <ecs/Entity.h>
#include "LightComponents.h"

namespace engine {

    class PhongLight : public Entity {

    public:
        PhongLight() = default;

        PhongLight(const std::string &tag, EntityContainer* container) : Entity(tag, container) {
            create();
        }

        PhongLight(EntityContainer* container) : Entity("PhongLight", container) {
            create();
        }

        ~PhongLight() = default;

    private:
        void create();

    };

    class DirectLight : public Entity {

    public:
        DirectLight() = default;

        DirectLight(const std::string &tag, EntityContainer* container) : Entity(tag, container) {
            create();
        }

        DirectLight(EntityContainer* container) : Entity("DirectLight", container) {
            create();
        }

        ~DirectLight() = default;

    private:
        void create();

    };

    class PointLight : public Entity {

    public:
        PointLight() = default;

        PointLight(const std::string &tag, EntityContainer* container) : Entity(tag, container) {
            create();
        }

        PointLight(EntityContainer* container) : Entity("PointLight", container) {
            create();
        }

        ~PointLight() = default;

    private:
        void create();

    };

    class FlashLight : public Entity {

    public:
        FlashLight() = default;

        FlashLight(const std::string &tag, EntityContainer* container) : Entity(tag, container) {
            create();
        }

        FlashLight(EntityContainer* container) : Entity("FlashLight", container) {
            create();
        }

        ~FlashLight() = default;

    private:
        void create();

    };

}
