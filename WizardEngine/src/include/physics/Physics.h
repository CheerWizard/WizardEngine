//
// Created by mecha on 01.09.2022.
//

#pragma once

#include <physics/Colliders.h>
#include <ecs/Scene.h>

namespace engine::physics {

    using namespace core;
    using namespace ecs;

    typedef std::function<void(entity_id, entity_id)> CollisionCallback;

    struct PhysicsCallback {
        // collision callbacks
        CollisionCallback onCollided = [](entity_id, entity_id){};
        CollisionCallback onNotCollided = [](entity_id, entity_id){};
    };

    class Physics final {

    public:
        static void onUpdate(time::Time dt);

    private:
        static void onIntersect(const IntersectData& intersectData, Registry& registry, entity_id entity1, entity_id entity2);

    public:
        static Ref<Scene> activeScene;
        static PhysicsCallback callback;
    };
}
