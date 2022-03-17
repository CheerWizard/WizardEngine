//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <core/Logger.h>

#include "entt/entt.hpp"

namespace engine {

    class Entity;

    class EntityContainer {

    public:
        void clearBatches();
        void clearInstances();
        void clear();
        [[nodiscard]] bool batchEmpty();
        [[nodiscard]] bool instanceEmpty();
        [[nodiscard]] bool isEmpty();
        [[nodiscard]] size_t batchSize();
        [[nodiscard]] size_t instanceSize();
        [[nodiscard]] size_t size();

    public:
        inline entt::registry& getBatchRegistry() {
            return batchRegistry;
        }

        inline entt::registry& getInstanceRegistry() {
            return instanceRegistry;
        }

    public:
        // todo find a way to copy whole registry
        template<typename T>
        void copy(entt::registry &src, entt::registry &dst) {
            auto view = src.view<T>();
            dst.emplace(view.data(), view.data() + view.size(), view.raw(), view.raw() + view.size());
        }

        template<typename T, typename... Args>
        inline bool addComponent(entt::registry& registry, const entt::entity& entityId, Args &&... args) {
            if (hasComponent<T>(registry, entityId)) {
                ENGINE_WARN("Entity {0} already has a component!", entityId);
                return false;
            }
            registry.emplace<T>(entityId, std::forward<Args>(args)...);
            return true;
        }

        template<typename T, typename... Args>
        inline bool updateComponent(entt::registry& registry, const entt::entity& entityId, Args &&... args) {
            if (!hasComponent<T>(registry, entityId)) {
                ENGINE_WARN("Entity {0} does not have a component!", entityId);
                return false;
            }
            registry.replace<T>(entityId, std::forward<Args>(args)...);
            return true;
        }

        // if component exists, we will draw it.
        // if component does not exist, we will add it.
        template<typename T, typename... Args>
        inline void setComponent(entt::registry& registry, const entt::entity& entityId, Args &&... args) {
            if (hasComponent<T>(registry, entityId)) {
                ENGINE_INFO("Updating component for entity id: {0}", entityId);
                registry.replace<T>(entityId, std::forward<Args>(args)...);
            } else {
                ENGINE_INFO("Adding component for entity id: {0}", entityId);
                registry.emplace<T>(entityId, std::forward<Args>(args)...);
            }
        }

        template<typename T>
        inline T& getComponent(entt::registry& registry, const entt::entity& entityId) {
            return *registry.try_get<T>(entityId);
        }

        template<typename T>
        inline T* getComponentPtr(entt::registry& registry, const entt::entity& entityId) {
            return registry.try_get<T>(entityId);
        }

        template<typename T>
        inline bool hasComponent(entt::registry& registry, const entt::entity& entityId) {
            return getComponentPtr<T>(registry, entityId) != nullptr;
        }

        template<typename T>
        inline bool removeComponent(entt::registry& registry, const entt::entity& entityId) {
            if (!hasComponent<T>(registry, entityId)) {
                ENGINE_WARN("Entity {0} does not have a component!", entityId);
                return false;
            }
            registry.remove<T>(entityId);
            return true;
        }

        template <typename... Components>
        std::vector<Entity> filter(entt::registry& registry) {
            return registry.template view<std::add_const<Components>...>();
        }

        template <typename... Components, typename Predicate>
        std::vector<entt::entity> filter(entt::registry& registry, Predicate &&predicate) {
            std::vector<entt::entity> filteredEntities;
            auto viewEntities = registry.template view<std::add_const_t<Components>...>();
            for (const auto& viewEntity : viewEntities) {
                if (predicate(viewEntities.template get<std::add_const_t<Components>>(viewEntity)...)) {
                    filteredEntities.push_back(viewEntity);
                }
            }
            return filteredEntities;
        }

        /**
         * If @param predicate returns true - element will be added to @param out_entities1.
         * Else - element will be added to @param out_entities2.
         * @returns @param out_entities1 and @param out_entities2 filled with Entity elements based on @param predicate.
         */
        template <typename... Components, typename Predicate>
        void filter(
                entt::registry& registry,
                Predicate &&predicate,
                std::vector<entt::entity> &out_entities1,
                std::vector<entt::entity> &out_entities2
        ) {
            auto viewEntities = registry.template view<std::add_const_t<Components>...>();
            for (const auto& viewEntity : viewEntities) {
                if (predicate(viewEntities.template get<std::add_const_t<Components>>(viewEntity)...)) {
                    out_entities1.push_back(viewEntity);
                } else {
                    out_entities2.push_back(viewEntity);
                }
            }
        }

    protected:
        entt::registry batchRegistry;
        entt::registry instanceRegistry;

        friend class Entity;
    };

    class Entity {

    public:
        Entity(const bool& instancingEnabled = false) :
        instancingEnabled(instancingEnabled) {}

        Entity(entt::entity id, EntityContainer* container, const bool& instancingEnabled = false) :
        id(id),
        container(container),
        instancingEnabled(instancingEnabled) {}

        Entity(EntityContainer* container, const bool& instancingEnabled = false) :
        container(container),
        instancingEnabled(instancingEnabled) {
            create();
        }

        Entity(const std::string &tag, EntityContainer* container, const bool& instancingEnabled = false) :
        container(container),
        instancingEnabled(instancingEnabled) {
            create(tag);
        }

        Entity(const Entity& entity) {
            id = entity.id;
            container = entity.container;
            instancingEnabled = entity.instancingEnabled;
        }

    public:
        [[nodiscard]] inline uint32_t getId() const {
            return (uint32_t) id;
        }

        [[nodiscard]] inline entt::entity getEnttId() const {
            return id;
        }

        template<typename T, typename... Args>
        inline bool add(Args &&... args);

        template<typename T, typename... Args>
        inline bool update(Args &&... args);

        template<typename T>
        inline T& get() const;

        template<typename T>
        inline T* getPtr() const;

        template<typename T>
        inline bool has() const;

        template<typename T>
        inline bool remove() const;

        template<typename T, typename... Args>
        inline void set(Args &&... args) const;

    public:
        operator const entt::entity&() const { return id; }
        operator bool() const { return id != entt::null; }
        operator uint32_t() const { return (uint32_t)id; }

        bool operator==(const Entity& other) const {
            return id == other.id && container == other.container;
        }

        bool operator!=(const Entity& other) const {
            return !(*this == other);
        }

    public:
        [[nodiscard]] inline entt::registry& getRegistry() const;
        void remove() const;

    public:
        bool instancingEnabled = false;

    protected:
        EntityContainer* container = nullptr;
        entt::entity id = { entt::null };

    private:
        void create();
        void create(const std::string &tag);

    };

    template<typename T, typename... Args>
    inline bool Entity::add(Args &&... args) {
        return container->addComponent<T>(getRegistry(), id, std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    inline bool Entity::update(Args &&... args) {
        return container->updateComponent<T>(getRegistry(), id, std::forward<Args>(args)...);
    }

    template<typename T>
    inline T& Entity::get() const {
        return container->getComponent<T>(getRegistry(), id);
    }

    template<typename T>
    inline bool Entity::has() const {
        return container->hasComponent<T>(getRegistry(), id);
    }

    template<typename T>
    inline bool Entity::remove() const {
        return container->removeComponent<T>(getRegistry(), id);
    }

    template<typename T, typename... Args>
    inline void Entity::set(Args &&... args) const {
        container->setComponent<T>(getRegistry(), id, std::forward<Args>(args)...);
    }

    template<typename T>
    inline T* Entity::getPtr() const {
        return container->getComponentPtr<T>(getRegistry(), id);
    }
}