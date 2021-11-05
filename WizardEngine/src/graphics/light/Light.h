//
// Created by mecha on 04.11.2021.
//

#pragma once

#include "../../ecs/Entity.h"
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

}
