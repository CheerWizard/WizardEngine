//
// Created by mecha on 29.10.2021.
//

#include <ecs/Entity.h>

namespace engine::ecs {

    void Entity::destroy() {
        container->getRegistry().deleteEntity(id);
    }

    bool EntityContainer::isEmpty() {
        return registry.empty_entity();
    }

    size_t EntityContainer::size() {
        return registry.entity_count();
    }

    void EntityContainer::clear() {
        registry.clear();
    }

    void Entity::create() {
       create("Empty");
    }

    void Entity::create(const std::string &tag) {
        id = container->getRegistry().createEntity();
        add<UUIDComponent>();
        add<TagComponent>(tag);
    }

    void Entity::setUUID(const uuid &uuid) {
        add<UUIDComponent>(uuid);
    }
}
