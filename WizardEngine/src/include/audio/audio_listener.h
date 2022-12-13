//
// Created by mecha on 24.04.2022.
//

#pragma once

#include <ecs/ecs.h>
#include <core/primitives.h>
#include <math/Math.h>

namespace engine::audio {

    using namespace core;

    component(Orientation) {
        serializable()
        math::vec3f lookVec = { 0, 0, -1 };
        math::vec3f upVec = { 0, 1, 0 };
    };

    component(AudioListenerComponent) {
        serializable()
        math::vec3f position;
        math::vec3f velocity;
        Orientation orientation;
    };

    class ENGINE_API Listener final {

    private:
        Listener() = default;
        ~Listener() = default;

    public:
        static void setPosition(const math::vec3f& position);
        static void setVelocity(const math::vec3f& velocity);
        static void setOrientation(const Orientation& orientation);
        static void setComponent(const AudioListenerComponent& component);
    };

}
