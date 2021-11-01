//
// Created by mecha on 24.10.2021.
//

#pragma once

#include "Entity.h"

namespace engine {

    class Family : public Entity {

    public:
        Family(entt::entity id, EntityContainer* container) : Entity(id, container) {}

        Family(entt::entity id,
               EntityContainer* container,
               const std::vector<Entity>& entities) :
               _entities(entities),
               Entity(id, container) {}

        Family(EntityContainer* container) : Entity(container) {
            create();
        }

        Family(const std::string &tag, EntityContainer* container) : Entity(container) {
            create(tag);
        }

        Family(const Family &family) = default;

    public:
        inline const std::vector<Entity>& getEntities() const {
            return _entities;
        }

        inline uint32_t getEntityCount() const {
            return _entities.size();
        };

    public:
        Entity createEntity(const std::string &tag);
        Entity createEntity2d(const std::string &tag);
        Entity createEntity3d(const std::string &tag);
        void deleteEntity(const Entity& entity);
        bool isEmpty() const;
        void clear();
        size_t size() const;
        const Entity& getEntity(const uint32_t &index);
        void addEntity(const Entity& entity);
        void setEntity(const Entity& entity);

    public:
        inline const Entity& operator[](const uint32_t &index) const;

    private:
        void create();
        void create(const std::string &tag);
        void destroy();

    private:
        std::vector<Entity> _entities = {};

    };

}
