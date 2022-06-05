//
// Created by mecha on 02.04.2022.
//

#include <ecs/ecs.h>

namespace engine::ecs {

    vector<ComponentType>* BaseComponent::componentTypes;

    u32 BaseComponent::registerComponentType(
            ComponentCreateFunction createFunction,
            ComponentDestroyFunction destroyFunction,
            component_size size
    ) {
        if (componentTypes == nullptr) {
            componentTypes = new vector<ComponentType>();
        }
        u32 id = componentTypes->size();
        componentTypes->emplace_back(ComponentType(createFunction, destroyFunction, size));
        return id;
    }

    Registry::~Registry() {
        clear();
    }

    entity_id Registry::createEntity() {
        // allocate entity
        auto* newEntity = new entity();
        // add entity
        newEntity->first = entities.size();
        entities.emplace_back(newEntity);
        return newEntity;
    }

    void Registry::deleteEntity(entity_id& entityId) {
        validate_entity("deleteEntity()", entityId,)
        // remove component ids and indexes
        auto& entityData = toEntityData(entityId);
        for (const auto& componentIdAndIndex : entityData) {
            removeComponentInternal(componentIdAndIndex.first, componentIdAndIndex.second);
        }
        // delete actual entity by index and replace it with last one
        u32 destIndex = toEntityIndex(entityId);
        u32 srcIndex = entities.size() - 1;
        delete entities[destIndex];
        entities[destIndex] = entities[srcIndex];
        entities[destIndex]->first = destIndex;
        entities.pop_back();
        entityId = invalid_entity_id;
    }

    // todo crash when trying to remove existing component
    void Registry::removeComponentInternal(component_id componentId, u32 componentIndex) {
        auto destroyFunction = BaseComponent::getDestroyFunction(componentId);
        component_data data = components[componentId];
        component_size typeSize = BaseComponent::getSize(componentId);
        u32 srcIndex = data.size() - typeSize;

        auto* destComponent = (BaseComponent*)&data[componentIndex];
        auto* srcComponent = (BaseComponent*)&data[srcIndex];
        destroyFunction(destComponent);

        if (componentIndex == srcIndex) {
            data.resize(srcIndex);
            return;
        }

        memcpy(destComponent, srcComponent, typeSize);

        auto& entityData = toEntityData(srcComponent->entityId);
        for (auto& componentIdAndIndex : entityData) {
            if (componentId == componentIdAndIndex.first && srcIndex == componentIdAndIndex.second) {
                componentIdAndIndex.second = componentIndex;
                break;
            }
        }

        data.resize(srcIndex);
    }

    size_t Registry::entity_count() {
        return entities.size();
    }

    bool Registry::empty_entity() {
        return entities.empty();
    }

    void Registry::clear() {
        for (auto &component: components) {
            auto typeSize = BaseComponent::getSize(component.first);
            auto destroyFunction = BaseComponent::getDestroyFunction(component.first);
            for (u32 i = 0; i < component.second.size(); i += typeSize) {
                destroyFunction((BaseComponent *) &component.second[i]);
            }
        }
        components.clear();
        entities.clear();
    }
}