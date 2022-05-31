//
// Created by mecha on 26.09.2021.
//

#pragma once

#include <io/Logger.h>
#include <ecs/Components.h>

namespace engine::ecs {

    class Entity;

    class EntityContainer {

    public:
        void clear();
        [[nodiscard]] bool isEmpty();
        [[nodiscard]] size_t size();

    public:
        inline entity_id createEntity() {
            return registry.createEntity();
        }

        inline Registry& getRegistry() {
            return registry;
        }

    public:
        template<typename T, typename... Args>
        inline bool addComponent(const entity_id& entityId, Args &&... args) {
            return registry.addComponent<T>(entityId, std::forward<Args>(args)...);
        }

        template<typename T, typename... Args>
        inline bool updateComponent(const entity_id& entityId, Args &&... args) {
            bool removed = registry.removeComponent<T>(entityId);
            registry.addComponent<T>(entityId, std::forward<Args>(args)...);
            return removed;
        }

        template<typename T>
        inline T* getComponent(const entity_id& entityId) {
            return registry.getComponent<T>(entityId);
        }

        template<typename T>
        inline bool hasComponent(const entity_id& entityId) {
            return registry.getComponent<T>(entityId);
        }

        template<typename T>
        inline bool removeComponent(const entity_id& entityId) {
            return registry.removeComponent<T>(entityId);
        }

    protected:
        Registry registry;

        friend class Entity;
    };

    class Entity {

    public:
        Entity() = default;

        Entity(EntityContainer* container, entity_id id) : id(id), container(container) {}

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
        [[nodiscard]] inline void* getId() const {
            return id;
        }

        [[nodiscard]] inline bool isValid() const {
            return id != invalid_entity_id;
        }

        [[nodiscard]] inline const uuid& getUUID() const {
            return get<UUIDComponent>()->uuid;
        }

        void setUUID(const uuid& uuid);

        template<typename T, typename... Args>
        inline bool add(Args &&... args);

        template<typename T, typename... Args>
        inline bool update(Args &&... args);

        template<typename T>
        inline T* get() const;

        template<typename T>
        [[nodiscard]] inline bool has() const;

        template<typename T>
        [[nodiscard]] inline bool remove() const;

    public:
        explicit operator const entity_id&() const { return id; }
        explicit operator bool() const { return id != invalid_entity_id; }
        explicit operator void *() const { return id; }

        bool operator==(const Entity& other) const {
            return id == other.id && container == other.container;
        }

        bool operator!=(const Entity& other) const {
            return !(*this == other);
        }

    public:
        void destroy();

    protected:
        EntityContainer* container = nullptr;
        entity_id id = invalid_entity_id;

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
    inline T* Entity::get() const {
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
}
