//
// Created by mecha on 26.09.2021.
//

#include "Scene.h"

#include "Components.h"
#include "../transform/TransformComponents.h"

namespace engine {

    Entity Scene::createEntity(const std::string &tag) {
        auto entity = Entity(tag, this);
        _entities.push_back(entity);
        return entity;
    }

    Entity Scene::createEntity2d(const std::string &tag) {
        auto entity = Entity(tag, this);
        entity.add<Transform2dComponent>(TransformComponents::newTransform2d());
        _entities.push_back(entity);
        return entity;
    }

    Entity Scene::createEntity3d(const std::string &tag) {
        auto entity = Entity(tag, this);
        entity.add<Transform3dComponent>(TransformComponents::newTransform3d());
        _entities.push_back(entity);
        return entity;
    }

    void Scene::deleteEntity(const Entity &entity) {
        deleteEntityFromRegistry(entity);
        auto it = std::find(_entities.begin(), _entities.end(), entity);
        _entities.erase(it);
    }

    void Scene::clear() {
        _registry.clear<>();
        _families.clear();
        _entities.clear();
    }

    Family Scene::createFamily(const std::string &tag) {
        auto family = Family(tag, this);
        _families.push_back(family);
        return family;
    }

    void Scene::deleteFamily(const Family &family) {
        deleteEntityFromRegistry(family);
        auto it = std::find(_families.begin(), _families.end(), family);
        _families.erase(it);
    }

    bool Scene::isEmpty() const {
        return _registry.empty<>();
    }

    void Scene::deleteEntityFromRegistry(const Entity &entity) {
        _registry.destroy(entity);
    }

    void Scene::addEntity(const Entity &entity) {
        _entities.push_back(entity);
    }

    void Scene::addFamily(const Entity &entity) {
        _families.push_back((const Family&) entity);
    }

    void Scene::setFamily(const Family &family) {
        auto it = std::find(_families.begin(), _families.end(), family);
        _families.emplace(it, family);
    }
}