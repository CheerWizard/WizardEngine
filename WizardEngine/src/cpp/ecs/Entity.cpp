//
// Created by mecha on 29.10.2021.
//

#include <ecs/Entity.h>
#include <ecs/Components.h>

namespace engine {

    void Entity::remove() const {
        getRegistry().destroy(*this);
    }

    bool EntityContainer::batchEmpty() {
        return batchRegistry.empty<>();
    }

    bool EntityContainer::instanceEmpty() {
        return instanceRegistry.empty<>();
    }

    bool EntityContainer::isEmpty() {
        return batchEmpty() && instanceEmpty();
    }

    size_t EntityContainer::batchSize() {
        return batchRegistry.size();
    }

    size_t EntityContainer::instanceSize() {
        return instanceRegistry.size();
    }

    size_t EntityContainer::size() {
        return batchSize() + instanceSize();
    }

    void EntityContainer::clearBatches() {
        batchRegistry.clear<>();
    }

    void EntityContainer::clearInstances() {
        instanceRegistry.clear<>();
    }

    void EntityContainer::clear() {
        clearBatches();
        clearInstances();
    }

    void Entity::create() {
       create("Empty");
    }

    void Entity::create(const std::string &tag) {
        id = getRegistry().create();
        add<TagComponent>(tag);
    }

    entt::registry& Entity::getRegistry() const {
        return instancingEnabled ? container->getInstanceRegistry() : container->getBatchRegistry();
    }

}