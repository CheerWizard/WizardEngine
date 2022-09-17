//
// Created by mecha on 01.09.2022.
//

#include <physics/Colliders.h>

namespace engine::physics {

    float IntersectData::distance() const {
        return direction.length();
    }

    PlaneCollider& PlaneCollider::normalize() {
        float magnitude = normal.length();
        normal /= magnitude;
        distance /= magnitude;
        return *this;
    }

    PlaneCollider::PlaneCollider(const vec2f &minV, const vec2f &maxV) : minV(minV, distance), maxV(maxV, distance) {
        calculateNormal();
    }

    void PlaneCollider::calculateNormal() {
        vec3f v1 = this->minV;
        vec3f v2 = { minV.x(), maxV.y(), distance };
        vec3f v3 = this->maxV;
        vec3f n1 = v2 - v1;
        vec3f n2 = v3 - v1;
        // use for counter-clockwise triangle
        normal = math::cross(n1, n2).normalize();
        // use for clockwise triangle
//        normal = math::cross(n2, n1).normalize();
    }

    void PlaneCollider::invalidate(const ModelMatrix3d& model) {
        minV = model.value * minV;
        maxV = model.value * maxV;
        calculateNormal();
    }

    IntersectData Intersections::intersect(const AABBCollider &aabb1, const AABBCollider &aabb2) {
        vec3f distance1 = aabb2.minExtents - aabb1.maxExtents;
        vec3f distance2 = aabb1.minExtents - aabb2.maxExtents;
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

    IntersectData Intersections::intersect(const AABBCollider &aabb, const SphereCollider &sphere) {
        return {};
    }

    IntersectData Intersections::intersect(const AABBCollider &aabb, const PlaneCollider &plane) {
        return {};
    }

    IntersectData Intersections::intersect(const SphereCollider &sphere1, const SphereCollider &sphere2) {
        float collisionDistance = sphere1.radius + sphere2.radius;
        vec3f direction = sphere2.center - sphere1.center;
        float centerDistance = direction.length();
        direction /= centerDistance;
        float distance = centerDistance - collisionDistance;
        return { distance < 0, distance * direction };
    }

    IntersectData Intersections::intersect(const SphereCollider &sphere, const PlaneCollider &plane) {
        float distanceToSphere = math::abs(math::dot(plane.normal, sphere.center) + plane.distance) - sphere.radius;
        return { distanceToSphere < 0, distanceToSphere * plane.normal };
    }

    IntersectData Intersections::intersect(const PlaneCollider &plane1, const PlaneCollider &plane2) {
        return {};
    }
}