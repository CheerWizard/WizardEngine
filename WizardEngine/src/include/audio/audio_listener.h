//
// Created by mecha on 24.04.2022.
//

#pragma once

#include <ecs/ecs.h>
#include <glm/glm.hpp>
#include <core/primitives.h>

namespace engine::audio {

    using namespace core;

    struct Orientation {
        glm::vec3 lookVec = { 0, 0, -1 };
        glm::vec3 upVec = { 0, 1, 0 };
    };

    component(AudioListenerComponent) {
        glm::vec3 position;
        glm::vec3 velocity;
        Orientation orientation;
    };

    class Listener final {

    private:
        Listener() = default;
        ~Listener() = default;

    public:
        static void setPosition(const glm::vec3& position);
        static void setVelocity(const glm::vec3& velocity);
        static void setOrientation(const Orientation& orientation);
        static void setComponent(const AudioListenerComponent& component);

    };

}
