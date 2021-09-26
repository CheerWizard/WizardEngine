//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "Scene.h"

#include "../core/Logger.h"
#include "../core/Assert.h"

#include <entt/entt.hpp>

namespace engine {

    class Entity {

    public:
        Entity() = default;

        Entity(entt::entity id, Scene* scene) :
        _id(id),
        _scene(scene) {}

        Entity(const Entity& other) = default;

    public:

        template<typename T, typename... Args>
        T& addComponent(Args &&... args) {
            ENGINE_ASSERT(!hasComponent<T>(), "Entity already has component!")
            return _scene->_entities.emplace<T>(_id, std::forward<Args>(args)...);
        }

        template<typename T>
        T& getComponent() {
            auto component = _scene->_entities.try_get<T>(_id);
            ENGINE_ASSERT(component != nullptr, "Entity does not have component!");
            return *component;
        }

        template<typename T>
        bool hasComponent() {
            return _scene->_entities.try_get<T>(_id) != nullptr;
        }

        template<typename T>
        void removeComponent() {
            ENGINE_ASSERT(hasComponent<T>(), "Entity does not have component!")
            _scene->_entities.remove<T>(_id);
        }

        operator bool() const { return _id != entt::null; }
        operator entt::entity() const { return _id; }
        operator uint32_t() const { return (uint32_t)_id; }

        bool operator==(const Entity& other) const {
            return _id == other._id && _scene == other._scene;
        }

        bool operator!=(const Entity& other) const {
            return !(*this == other);
        }

    private:
        Scene* _scene = nullptr;
        entt::entity _id = { entt::null };

    };

}