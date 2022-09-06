//
// Created by mecha on 01.09.2022.
//

#pragma once

#include <math/Math.h>
#include <ecs/ecs.h>
#include <math/ModelMatrices.h>

namespace engine::physics {

    using namespace math;

    component(CollisionTransform) {
        ModelMatrix3d model;
        bool bindOrigin = true;

        CollisionTransform() = default;
        CollisionTransform(const vec3f& position, const vec3f& rotation, const vec3f& scale)
        : model(position, rotation, scale) {}
        CollisionTransform(const ModelMatrix3d& model) : model(model) {}
    };

    component(SphereCollider) {
        vec3f center = { 0, 0, 0 };
        float radius = 0;

        SphereCollider() = default;
        SphereCollider(float x, float y, float z, float radius) : center(x, y, z), radius(radius) {}
        SphereCollider(const vec3f& center, float radius) : center(center), radius(radius) {}
    };

    // AABB - Axis Aligned Bounding Box
    component(AABBCollider) {
        vec3f minExtents;
        vec3f maxExtents;

        AABBCollider() = default;
    };

    component(PlaneCollider) {
        vec3f normal;
        float distance = 0;
        vec3f minV;
        vec3f maxV;

        PlaneCollider() = default;
        PlaneCollider(const vec2f& minV, const vec2f& maxV);
        PlaneCollider& normalize();

        void calculateNormal();
        void invalidate(const ModelMatrix3d& model);
    };

    component(Velocity) {
        vec3f velocity;
        bool flipped = false;

        Velocity() = default;
        Velocity(float x, float y, float z) : velocity(x, y, z) {}
        Velocity(const vec3f& velocity) : velocity(velocity) {}
    };

    struct IntersectData {
        bool intersected;
        vec3f direction;

        [[nodiscard]] float distance() const;
    };

    class Intersections final {

    public:
        static IntersectData intersect(const AABBCollider& aabb1, const AABBCollider& aabb2);
        static IntersectData intersect(const AABBCollider& aabb, const SphereCollider& sphere);
        static IntersectData intersect(const AABBCollider& aabb, const PlaneCollider& plane);
        static IntersectData intersect(const SphereCollider& sphere1, const SphereCollider& sphere2);
        static IntersectData intersect(const SphereCollider& sphere, const PlaneCollider& plane);
        static IntersectData intersect(const PlaneCollider& plane1, const PlaneCollider& plane2);
    };

}