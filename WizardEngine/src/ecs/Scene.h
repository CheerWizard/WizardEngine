//
// Created by mecha on 26.09.2021.
//

#pragma once

#include "Family.h"

namespace engine {

    class Scene : public EntityContainer {

    public:
        Scene() = default;
        ~Scene() {
            clear();
        }

    public:
        inline const uint32_t& getTextureId() const {
            return _textureId;
        }

        inline void setTextureId(const uint32_t &textureId) {
            _textureId = textureId;
        }

        inline const std::vector<Family>& getFamilies() {
            return _families;
        }

        inline const std::vector<Entity>& getEntities() {
            return _entities;
        }

        inline entt::registry& getRegistry() override {
            return _registry;
        }

    public:
        Family createFamily(const std::string &tag);
        void deleteFamily(const Family& family);
        void clear();
        bool isEmpty() const;

        Entity createEntity(const std::string &tag) override;
        Entity createEntity2d(const std::string &tag) override;
        Entity createEntity3d(const std::string &tag) override;
        void deleteEntityFromRegistry(const Entity& entity) override;
        void deleteEntity(const Entity& entity) override;

        void addEntity(const Entity &entity) override;
        void addFamily(const Entity &entity) override;
        void setFamily(const Family &family);

    private:
        entt::registry _registry; // registry of all entities for this Scene.
        std::vector<Family> _families;
        std::vector<Entity> _entities; // entities that are not included to Family.
        uint32_t _textureId; // id of texture that stores pixels of this Scene.
    };

}