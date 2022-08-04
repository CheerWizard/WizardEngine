//
// Created by mecha on 24.04.2022.
//

#include <audio/audio_listener.h>
#include <audio/audio_core.h>

namespace engine::audio {

    using namespace math;

    void Orientation::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "Orientation";
        yaml::serialize(out, "look", lookVec);
        yaml::serialize(out, "up", upVec);
        out << YAML::EndMap;
    }

    void Orientation::deserialize(const YAML::Node &parent) {
        auto root = parent["Orientation"];
        if (root) {
            lookVec = root["look"].as<vec3f>();
            upVec = root["up"].as<vec3f>();
        }
    }

    void AudioListenerComponent::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "AudioListenerComponent";

        orientation.serialize(out);
        yaml::serialize(out, "position", position);
        yaml::serialize(out, "velocity", velocity);

        out << YAML::EndMap;
    }

    void AudioListenerComponent::deserialize(const YAML::Node &parent) {
        auto root = parent["AudioListenerComponent"];
        if (root) {
            orientation.deserialize(parent);
            position = root["position"].as<vec3f>();
            velocity = root["velocity"].as<vec3f>();
        }
    }

    void Listener::setComponent(const AudioListenerComponent &component) {
        setPosition(component.position);
        setVelocity(component.velocity);
        setOrientation(component.orientation);
    }

    void Listener::setPosition(const vec3f &position) {
        alCall(alListener3f, AL_POSITION, position.x(), position.y(), position.z());
    }

    void Listener::setVelocity(const vec3f &velocity) {
        alCall(alListener3f, AL_VELOCITY, velocity.x(), velocity.y(), velocity.z());
    }

    void Listener::setOrientation(const Orientation &orientation) {
        const auto& lookVec = orientation.lookVec;
        const auto& upVec = orientation.upVec;
        f32 value[6] = {
                lookVec.x(), lookVec.y(), lookVec.z(),
                upVec.x(), upVec.y(), upVec.z()
        };
        alCall(alListenerfv, AL_ORIENTATION, value);
    }

}