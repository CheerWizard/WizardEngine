//
// Created by mecha on 31.05.2022.
//

#include <ecs/Scene.h>

namespace engine::ecs {

    Scene::~Scene() {
        clear();
    }

    Entity Scene::findEntity(const uuid &uuid) {
        UUIDComponent uuidComponent(uuid);
        entity_id entityId = registry.findEntity<UUIDComponent>([&uuidComponent](UUIDComponent* found) {
            return *found == uuidComponent;
        });
        return { this, entityId };
    }

    Entity Scene::findEntity(const UUIDComponent& uuid) {
        entity_id entityId = registry.findEntity<UUIDComponent>([&uuid](UUIDComponent* found) {
            return *found == uuid;
        });
        return { this, entityId };
    }

    Entity Scene::findEntity(u64 uuid) {
        UUIDComponent uuidComponent((engine::uuid(uuid)));
        entity_id entityId = registry.findEntity<UUIDComponent>([&uuidComponent](UUIDComponent* found) {
            return *found == uuidComponent;
        });
        return { this, entityId };
    }

}