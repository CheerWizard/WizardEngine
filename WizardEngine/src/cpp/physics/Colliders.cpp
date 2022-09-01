//
// Created by mecha on 01.09.2022.
//

#include <physics/Colliders.h>

namespace engine::physics {

    IntersectData SphereCollider::intersect(const SphereCollider& other) const {
        float collisionDistance = radius + other.radius;
        vec3f direction = other.center - center;
        float centerDistance = direction.length();
        direction /= centerDistance;
        float distance = centerDistance - collisionDistance;
        return { distance < 0, distance * direction };
    }

    IntersectData AABBCollider::intersect(const AABBCollider &other) const {
        vec3f distance1 = other.minExtents - maxExtents;
        vec3f distance2 = minExtents - other.maxExtents;
        // find max axis distance between AABB colliders
        vec3f maxDistance = {
                distance1[0] > distance2[0] ? distance1[0] : distance2[0],
                distance1[1] > distance2[1] ? distance1[1] : distance2[1],
                distance1[2] > distance2[2] ? distance1[2] : distance2[2]
        };
        float maxAxisDistance = maxDistance.x();
        if (maxAxisDistance < maxDistance.y()) {
            maxAxisDistance = maxDistance.y();
        }
        if (maxAxisDistance < maxDistance.z()) {
            maxAxisDistance = maxDistance.z();
        }

        return { maxAxisDistance < 0, maxDistance };
    }

    PlaneCollider& PlaneCollider::normalize() {
        float magnitude = normal.length();
        normal /= magnitude;
        distance /= magnitude;
        return *this;
    }

    IntersectData PlaneCollider::intersect(const SphereCollider &sphere) const {
        float distanceToSphere = math::abs(math::dot(normal, sphere.center) + distance) - sphere.radius;
        return { distanceToSphere < 0, distanceToSphere * normal };
    }

    float IntersectData::distance() const {
        return direction.length();
    }
}