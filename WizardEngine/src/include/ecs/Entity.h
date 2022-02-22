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
        virtual ~EntityContainer() = default;

    public:
        void deleteEntity(const Entity& entity);
        [[nodiscard]] entt::entity createEntityId();

        virtual void clear();
        [[nodiscard]] virtual bool isEmpty();
        [[nodiscard]] size_t size();

    public:
        inline entt::registry& getRegistry() {
            return registry;
        }

    public:
        // todo find a way to copy whole registry
        template<typename T>
        void copy(entt::registry &src, entt::registry &dst) {
            auto view = src.view<T>();
            dst.emplace(view.data(), view.data() + view.size(), view.raw(), view.raw() + view.size());
        }

        template<typename T, typename... Args>
        inline bool addComponent(const entt::entity& entityId, Args &&... args) {
            if (hasComponent<T>(entityId)) {
                ENGINE_WARN("Entity {0} already has a component!", entityId);
                return false;
            }
            registry.emplace<T>(entityId, std::forward<Args>(args)...);
            return true;
        }

        template<typename T, typename... Args>
        inline bool updateComponent(const entt::entity& entityId, Args &&... args) {
            if (!hasComponent<T>(entityId)) {
                ENGINE_WARN("Entity {0} does not have a component!", entityId);
                return false;
            }
            registry.replace<T>(entityId, std::forward<Args>(args)...);
            return true;
        }

        // if component exists, we will update it.
        // if component does not exist, we will add it.
        template<typename T, typename... Args>
        inline void setComponent(const entt::entity& entityId, Args &&... args) {
            if (hasComponent<T>(entityId)) {
                ENGINE_INFO("Updating component for entity id: {0}", entityId);
                registry.replace<T>(entityId, std::forward<Args>(args)...);
            } else {
                ENGINE_INFO("Adding component for entity id: {0}", entityId);
                registry.emplace<T>(entityId, std::forward<Args>(args)...);
            }
        }

        template<typename T>
        inline T& getComponent(const entt::entity& entityId) {
            return *registry.try_get<T>(entityId);
        }

        template<typename T>
        inline T* getComponentPtr(const entt::entity& entityId) {
            return registry.try_get<T>(entityId);
        }

        template<typename T>
        inline bool hasComponent(const entt::entity& entityId) {
            return getComponentPtr<T>(entityId) != nullptr;
        }

        template<typename T>
        inline bool removeComponent(const entt::entity& entityId) {
            if (!hasComponent<T>(entityId)) {
                ENGINE_WARN("Entity {0} does not have a component!", entityId);
                return false;
            }
            registry.remove<T>(entityId);
            return true;
        }

        template <typename... Components>
        std::vector<Entity> filter() {
            return registry.template view<std::add_const<Components>...>();
        }

        template <typename... Components, typename Predicate>
        std::vector<entt::entity> filter(Predicate &&predicate) {
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
        entt::registry registry;

        friend class Entity;
    };

    class Entity {

    public:
        Entity() = default;

        Entity(entt::entity id, EntityContainer* container) :
        id(id),
        container(container) {}

        Entity(EntityContainer* container) : container(container) {
            create();
        }

        Entity(const std::string &tag, EntityContainer* container) : container(container) {
            create(tag);
        }

        Entity(const Entity& entity) {
            id = entity.id;
            container = entity.container;
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

    protected:
        EntityContainer* container = nullptr;
        entt::entity id = { entt::null };

    private:
        void create();
        void create(const std::string &tag);

    };

    template<typename T, typename... Args>
    inline bool Entity::add(Args &&... args) {
        return container->addComponent<T>(id, std::forward<Args>(args)...);
    }

    template<typename T, typename... Args>
    inline bool Entity::update(Args &&... args) {
        return container->updateComponent<T>(id, std::forward<Args>(args)...);
    }

    template<typename T>
    inline T& Entity::get() const {
        return container->getComponent<T>(id);
    }

    template<typename T>
    inline bool Entity::has() const {
        return container->hasComponent<T>(id);
    }

    template<typename T>
    inline bool Entity::remove() const {
        return container->removeComponent<T>(id);
    }

    template<typename T, typename... Args>
    inline void Entity::set(Args &&... args) const {
        container->setComponent<T>(id, std::forward<Args>(args)...);
    }

    template<typename T>
    inline T* Entity::getPtr() const {
        return container->getComponentPtr<T>(id);
    }
}