//
// Created by mecha on 29.10.2021.
//

#include "Entity.h"

#include "Components.h"

namespace engine {

    entt::entity EntityContainer::createEntityId() {
        return getRegistry().create();
    }

    void Entity::create() {
       create("Empty");
    }

    void Entity::create(const std::string &tag) {
        id = container->createEntityId();
        add<TagComponent>(tag);
    }

    void Entity::destroy() {
        container = nullptr;
    }
}