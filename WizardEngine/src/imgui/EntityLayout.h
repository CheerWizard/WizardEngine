//
// Created by mecha on 03.10.2021.
//

#pragma once

#include "SceneLayout.h"
#include "../ecs/Entity.h"
#include "../core/String.h"

namespace engine {

    class EntityLayout : public SceneLayout {

    public:
        EntityLayout(Scene* scene, const SceneLayoutProps &props) : SceneLayout(scene, props) {}

        ~EntityLayout() override {
            destroy();
        }

    public:
        void clear();
        Entity create(const std::string &tag);
        Entity create();
        void remove(const Entity &entity);

        void onUpdate(Time deltaTime) override;

    private:
        void destroy();

    };

}