//
// Created by mecha on 24.10.2021.
//

#pragma once

#include "Entity.h"

namespace engine {
    // Family is a group of Entity that behaves as an Entity and EntityContainer
    // Useful in cases when you need to apply the same Component to several Entities.
    // Instead of copying same Component for each Entity, we can just apply it to Family
    // and it will be processed for each one in the registry.
    // It will consume less memory and will be faster for iteration.
    class Family : public Entity, public EntityContainer {

    public:
        Family(const std::string &tag = "Empty") {
            create(tag);
        }

        ~Family() override {
            clear();
        }

    private:
        void create(const std::string &tag);

    };

}
