//
// Created by mecha on 29.10.2021.
//

#include <ecs/Entity.h>
#include <ecs/Components.h>

namespace engine {

    entt::entity EntityContainer::createEntityId() {
        return registry.create();
    }

    void EntityContainer::deleteEntity(const Entity &entity) {
        registry.destroy(entity);
    }

    void EntityContainer::clear() {
        registry.clear<>();
    }

    bool EntityContainer::isEmpty() {
        return registry.empty<>();
    }

    size_t EntityContainer::size() {
        return registry.size();
    }

    void Entity::create() {
       create("Empty");
    }

    void Entity::create(const std::string &tag) {
        id = container->createEntityId();
        add<TagComponent>(tag);
    }

}