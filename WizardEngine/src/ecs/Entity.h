//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "Scene.h"

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
            return _scene->addComponent<T>(_id, std::forward<Args>(args)...);
        }

        template<typename T>
        T& getComponent() {
            return _scene->getComponent<T>(_id);
        }

        template<typename T>
        bool hasComponent() {
            return _scene->hasComponent<T>(_id);
        }

        template<typename T>
        void removeComponent() {
            _scene->removeComponent<T>(_id);
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
        entt::entity _id = { entt::null };
        Scene* _scene = nullptr;

    };

}