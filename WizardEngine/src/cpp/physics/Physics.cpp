//
// Created by mecha on 01.09.2022.
//

#include <physics/Physics.h>
#include <graphics/transform/TransformComponents.h>

namespace engine::physics {

    using namespace graphics;

    Ref<Scene> Physics::activeScene;
    PhysicsCallback Physics::callback;

    void Physics::onUpdate(time::Time dt) {
        auto& registry = activeScene->getRegistry();
        // collision detection
        // Sphere - Sphere collision
        registry.eachCompare<SphereCollider>([&registry](SphereCollider* sphere1, SphereCollider* sphere2) {
            onIntersect(sphere1->intersect(*sphere2), registry, sphere1->entityId, sphere2->entityId);
        });
        // AABB - AABB collision
        registry.eachCompare<AABBCollider>([&registry](AABBCollider* aabb1, AABBCollider* aabb2) {
            onIntersect(aabb1->intersect(*aabb2), registry, aabb1->entityId, aabb2->entityId);
        });
        // Plane - Sphere collision
        registry.eachCompare<PlaneCollider, SphereCollider>([&registry](PlaneCollider* plane, SphereCollider* sphere) {
            onIntersect(plane->intersect(*sphere), registry, plane->entityId, sphere->entityId);
        });
        // simulation
        registry.each<Transform3dComponent, Velocity>([&dt, &registry](
                Transform3dComponent* transform,
                Velocity* velocity
        ) {
            transform->modelMatrix.position += velocity->velocity * dt.getMilliseconds();
            transform->modelMatrix.apply();
            // simulate sphere colliders
            auto sphere = registry.getComponent<SphereCollider>(transform->entityId);
            if (sphere) {
                sphere->center = transform->modelMatrix.position;
                return;
            }
            // simulate AABB colliders
            auto aabb = registry.getComponent<AABBCollider>(transform->entityId);
            if (aabb) {

                return;
            }
            // simulate Plane colliders
            auto plane = registry.getComponent<PlaneCollider>(transform->entityId);
            if (plane) {

                return;
            }
        });
    }

    void Physics::onIntersect(const IntersectData &intersectData, Registry& registry,
                              entity_id entity1, entity_id entity2) {
        auto velocity1 = registry.getComponent<Velocity>(entity1);
        auto velocity2 = registry.getComponent<Velocity>(entity2);
        if (intersectData.intersected) {
            vec3f direction = intersectData.direction.normalize();
            vec3f otherDirection = math::reflect(direction, velocity1->velocity.normalize());
            if (!velocity1->flipped) {
                velocity1->flipped = true;
                velocity1->velocity = math::reflect(velocity1->velocity, otherDirection);
            }
            if (!velocity2->flipped) {
                velocity2->flipped = true;
                velocity2->velocity = math::reflect(velocity2->velocity, direction);
            }
            callback.onCollided(entity1, entity2);
        } else {
            velocity1->flipped = false;
            velocity2->flipped = false;
            callback.onNotCollided(entity1, entity2);
        }
    }

}