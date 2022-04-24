//
// Created by mecha on 24.04.2022.
//

#include <audio/audio_listener.h>
#include <audio/audio_core.h>

namespace engine::audio {

    void Listener::setComponent(const AudioListenerComponent &component) {
        setPosition(component.position);
        setVelocity(component.velocity);
        setOrientation(component.orientation);
    }

    void Listener::setPosition(const glm::vec3 &position) {
        alCall(alListener3f, AL_POSITION, position.x, position.y, position.z);
    }

    void Listener::setVelocity(const glm::vec3 &velocity) {
        alCall(alListener3f, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
    }

    void Listener::setOrientation(const Orientation &orientation) {
        const auto& lookVec = orientation.lookVec;
        const auto& upVec = orientation.upVec;
        f32 value[6] = {
                lookVec.x, lookVec.y, lookVec.z,
                upVec.x, upVec.y, upVec.z
        };
        alCall(alListenerfv, AL_ORIENTATION, value);
    }

}