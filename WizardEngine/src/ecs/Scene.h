//
// Created by mecha on 26.09.2021.
//
#pragma once

#include "string"

#include "entt/entt.hpp"

namespace engine {

    class Entity;

    class Scene {

    public:
        Scene() = default;
        ~Scene() = default;

    public:
        inline entt::registry& getEntities() {
            return _entities;
        }

    public:
        Entity createEntity();
        Entity createEntity(const std::string &tag);
        void deleteEntity(const Entity& entity);

    private:
        entt::registry _entities;
        // in order to hide strong association between Scene and Entity from all other classes.
        // Entity will be able to call private functionality from Scene.
        friend class Entity;
    };

}