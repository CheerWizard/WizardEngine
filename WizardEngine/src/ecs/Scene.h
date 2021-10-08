//
// Created by mecha on 26.09.2021.
//
#pragma once

#include "../core/Assert.h"
#include "../core/String.h"

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
        void clear();

        template<typename T, typename... Args>
        inline T& addComponent(const entt::entity& entityId, Args &&... args) {
            ENGINE_ASSERT(!hasComponent<T>(entityId), "Entity already has component!")
            return _entities.emplace<T>(entityId, std::forward<Args>(args)...);
        }

        template<typename T>
        inline T& getComponent(const entt::entity& entityId) {
            return *_entities.try_get<T>(entityId);
        }

        template<typename T>
        inline bool hasComponent(const entt::entity& entityId) const {
            return _entities.try_get<T>(entityId) != nullptr;
        }

        template<typename T>
        inline void removeComponent(const entt::entity& entityId) {
            ENGINE_ASSERT(hasComponent<T>(entityId), "Entity does not have component!")
            _entities.remove<T>(entityId);
        }

        inline const uint32_t& getTextureId() const {
            return _textureId;
        }

        inline void setTextureId(const uint32_t &textureId) {
            _textureId = textureId;
        }

    private:
        entt::registry _entities;
        uint32_t _textureId;

    };

}