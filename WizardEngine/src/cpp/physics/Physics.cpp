//
// Created by mecha on 01.09.2022.
//

#include <physics/Physics.h>
#include <graphics/transform/TransformComponents.h>
#include <profiler/Profiler.h>

namespace engine::physics {

    using namespace graphics;

    Ref<Scene> Physics::activeScene;
    PhysicsCallback Physics::callback;
    bool Physics::isEnabled = true;

    void Physics::onUpdate(time::Time dt) {
        PROFILE_FUNCTION();

        if (!isEnabled) return;
        auto& registry = activeScene->getRegistry();
        // collision detection
        // AABB - AABB collision
        registry.eachPair<AABBCollider>([&registry](AABBCollider* aabb1, AABBCollider* aabb2) {
            handleIntersectData(Intersections::intersect(*aabb1, *aabb2), registry, aabb1->entityId, aabb2->entityId);
        });
        // AABB - Sphere collision
        registry.eachPair<AABBCollider, SphereCollider>([&registry](AABBCollider* aabb, SphereCollider* sphere) {
            handleIntersectData(Intersections::intersect(*aabb, *sphere), registry, aabb->entityId, sphere->entityId);
        });
        // AABB - Plane collision
        registry.eachPair<AABBCollider, PlaneCollider>([&registry](AABBCollider* aabb, PlaneCollider* plane) {
            handleIntersectData(Intersections::intersect(*aabb, *plane), registry, aabb->entityId, plane->entityId);
        });
        // Sphere - Sphere collision
        registry.eachPair<SphereCollider>([&registry](SphereCollider* sphere1, SphereCollider* sphere2) {
            handleIntersectData(Intersections::intersect(*sphere1, *sphere2), registry, sphere1->entityId, sphere2->entityId);
        });
        // Sphere - Plane collision
        registry.eachPair<SphereCollider, PlaneCollider>([&registry](SphereCollider* sphere, PlaneCollider* plane) {
            handleIntersectData(Intersections::intersect(*sphere, *plane), registry, sphere->entityId, plane->entityId);
        });
        // Plane - Plane collision
        registry.eachPair<PlaneCollider>([&registry](PlaneCollider* plane1, PlaneCollider* plane2) {
            handleIntersectData(Intersections::intersect(*plane1, *plane2), registry, plane1->entityId, plane2->entityId);
        });
        // simulation
        registry.each<Transform3dComponent, Velocity>([&dt, &registry](
                Transform3dComponent* transform,
                Velocity* velocity
        ) {
            transform->modelMatrix.position += velocity->velocity * dt;
            transform->modelMatrix.apply();

            // simulate sphere colliders
            auto sphere = registry.getComponent<SphereCollider>(transform->entityId);
            if (sphere) {
                sphere->center = transform->modelMatrix.position;
                return;
            }

            auto* collisionTransform = registry.getComponent<CollisionTransform>(transform->entityId);
            if (collisionTransform) {
                auto& collisionPosition = collisionTransform->model.position;
                if (collisionTransform->bindOrigin) {
                    collisionPosition = transform->modelMatrix.position;
                    collisionTransform->model.apply();
                }
                // simulate sphere colliders
                auto sphere = registry.getComponent<SphereCollider>(transform->entityId);
                if (sphere) {
                    sphere->center = collisionPosition;
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
                    plane->invalidate(collisionTransform->model);
                    return;
                }
            }
        });
    }

    void Physics::handleIntersectData(
            const IntersectData &intersectData, Registry& registry,
            entity_id entity1, entity_id entity2
    ) {
        auto velocity1 = registry.getComponent<Velocity>(entity1);
        auto velocity2 = registry.getComponent<Velocity>(entity2);
        if (intersectData.intersected) {
            if (velocity1 && velocity2) {
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
            }
            callback.onCollided(entity1, entity2);
        } else {
            if (velocity1 && velocity2) {
                velocity1->flipped = false;
                velocity2->flipped = false;
            }
            callback.onNotCollided(entity1, entity2);
        }
    }

}