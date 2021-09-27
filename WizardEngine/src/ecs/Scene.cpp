//
// Created by mecha on 26.09.2021.
//

#include "Scene.h"
#include "Components.h"
#include "Entity.h"

namespace engine {

    Entity Scene::createEntity(const std::string &name) {
        auto entity = createEntity();
        auto& tagComponent = entity.addComponent<TagComponent>();
        tagComponent.tag = name.empty() ? "Entity" : name;
        return entity;
    }

    void Scene::deleteEntity(const Entity &entity) {
        _entities.destroy(entity);
    }

    Entity Scene::createEntity() {
        return Entity { _entities.create(), this };
    }

}