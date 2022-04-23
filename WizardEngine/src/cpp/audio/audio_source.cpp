//
// Created by mecha on 22.04.2022.
//

#include <audio/audio_source.h>
#include <audio/audio_core.h>

namespace engine::audio {

    void Source::create() {
        alCall(alGenSources, 1, &id);
        buffer.create();
    }

    void Source::destroy() {
        alCall(alDeleteSources, 1, &id);
        buffer.destroy();
    }

    void Source::recreate() {
        buffer.recreate();
        destroy();
        create();
    }

    void Source::setPitch(f32 pitch) const {
        alCall(alSourcef, id, AL_PITCH, pitch);
    }

    void Source::setGain(f32 gain) const {
        alCall(alSourcef, id, AL_GAIN, gain);
    }

    void Source::setPosition(const glm::vec3 &position) const {
        alCall(alSource3f, id, AL_POSITION, position.x, position.y, position.z);
    }

    void Source::setVelocity(const glm::vec3 &velocity) const {
        alCall(alSource3f, id, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
    }

    void Source::setLooping(bool enabled) const {
        alCall(alSourcei, id, AL_LOOPING, enabled);
    }

    void Source::load(const io::AudioData &audioData) {
        buffer.load(audioData);
    }

    void Source::setBuffer() const {
        alCall(alSourcei, id, AL_BUFFER, buffer.get());
    }
}
