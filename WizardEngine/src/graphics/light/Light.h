//
// Created by mecha on 04.11.2021.
//

#pragma once

#include "../../ecs/Entity.h"
#include "LightComponents.h"

namespace engine {

    class RegularLight : public Entity {

    public:
        RegularLight() = default;

        RegularLight(const std::string &tag, EntityContainer* container) : Entity(tag, container) {
            create();
        }

        RegularLight(EntityContainer* container) : Entity("RegularLight", container) {
            create();
        }

        ~RegularLight() = default;

    private:
        void create();

    };

}
