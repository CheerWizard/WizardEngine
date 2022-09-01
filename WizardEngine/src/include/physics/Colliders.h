//
// Created by mecha on 01.09.2022.
//

#pragma once

#include <math/Math.h>
#include <ecs/ecs.h>

namespace engine::physics {

    using namespace math;

    struct IntersectData {
        bool intersected;
        vec3f direction;

        float distance() const;
    };

    component(SphereCollider) {
        vec3f center = { 0, 0, 0 };
        float radius = 0;

        SphereCollider() = default;
        SphereCollider(float x, float y, float z, float radius) : center(x, y, z), radius(radius) {}
        SphereCollider(const vec3f& center, float radius) : center(center), radius(radius) {}

        [[nodiscard]] IntersectData intersect(const SphereCollider& other) const;
    };

    // AABB - Axis Aligned Bounding Box
    component(AABBCollider) {
        vec3f minExtents;
        vec3f maxExtents;

        AABBCollider() = default;

        [[nodiscard]] IntersectData intersect(const AABBCollider& other) const;
    };

    component(PlaneCollider) {
        vec3f normal;
        float distance = 0;

        PlaneCollider() = default;

        PlaneCollider& normalize();
        [[nodiscard]] IntersectData intersect(const SphereCollider& sphere) const;
    };

    component(Velocity) {
        vec3f velocity;
        bool flipped = false;
        Velocity() = default;
        Velocity(float x, float y, float z) : velocity(x, y, z) {}
        Velocity(const vec3f& velocity) : velocity(velocity) {}
    };

}