//
// Created by mecha on 29.10.2021.
//

#include <ecs/Entity.h>
#include <ecs/Components.h>

namespace engine {

    void Entity::remove() const {
        container->getRegistry().destroy(*this);
    }

    bool EntityContainer::isEmpty() {
        return registry.empty<>();
    }

    size_t EntityContainer::size() {
        return registry.size();
    }

    void EntityContainer::clear() {
        registry.clear<>();
    }

    void Entity::create() {
       create("Empty");
    }

    void Entity::create(const std::string &tag) {
        id = container->getRegistry().create();
        add<TagComponent>(tag);
    }
}
