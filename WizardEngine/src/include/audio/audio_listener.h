//
// Created by mecha on 24.04.2022.
//

#pragma once

#include <ecs/ecs.h>
#include <core/primitives.h>
#include <math/Math.h>

namespace engine::audio {

    using namespace core;

    struct Orientation : io::Serializable {

        ENGINE_API void serialize(YAML::Emitter &out) override;
        ENGINE_API void deserialize(const YAML::Node &parent) override;

        math::vec3f lookVec = { 0, 0, -1 };
        math::vec3f upVec = { 0, 1, 0 };
    };

    serialize_component(AudioListenerComponent) {
        math::vec3f position;
        math::vec3f velocity;
        Orientation orientation;

        ENGINE_API void serialize(YAML::Emitter &out) override;
        ENGINE_API void deserialize(const YAML::Node &parent) override;
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
