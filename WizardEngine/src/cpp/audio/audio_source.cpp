//
// Created by mecha on 22.04.2022.
//

#include <audio/audio_source.h>
#include <audio/audio_core.h>

namespace engine::audio {

    void Source::create(const u8& bufferCount) {
        alCall(alGenSources, 1, &id);
        if (bufferCount == 1) {
            Buffer buffer;
            buffer.create();
            buffers.emplace_back(buffer);
        } else {
            u32* ids = new u32[bufferCount];
            Buffer::create(ids, bufferCount);

            for (u32 i = 0; i < bufferCount; i++) {
                buffers.emplace_back(ids[i]);
            }

            delete[] ids;
        }
    }

    void Source::destroy() {
        clearBuffers();
        alCall(alDeleteSources, 1, &id);
    }

    void Source::recreate() {
        u32 bufferCount = buffers.size();
        destroy();
        create(bufferCount);
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
        for (auto buffer : buffers) {
            buffer.load(audioData);
        }
    }

    void Source::setBuffer(const u32& bufferIndex) const {
        alCall(alSourcei, id, AL_BUFFER, buffers.at(bufferIndex).get());
    }

    void Source::queueBuffers() const {
        alCall(alSourceQueueBuffers, id, buffers.size(), &buffers[0].get());
    }

    void Source::load(const u32 &bufferIndex, const io::AudioData &audioData) {
        buffers[bufferIndex].load(audioData);
    }

    void Source::loadStream(const std::string& filepath) {
        if (io::AudioFile::isOpen()) io::AudioFile::close();

        io::AudioFile::open(filepath.c_str());
        format = io::AudioFile::readWavHeaders(filepath.c_str());
        cursor = { static_cast<u8>(format.size / kb_512), kb_512 };

        create(cursor.bufferCount - buffers.size());

        for (u32 i = 0 ; i < buffers.size() ; i++) {
            char* data = io::AudioFile::streamWav((s64) (i * cursor.bufferSize), (s64) cursor.bufferSize);
            io::AudioData subData {
                {static_cast<s32>(cursor.bufferSize), format.frequency, format.channels },
                data,
            };

            buffers[i].load(subData);
            delete[] data;
        }
    }

    void Source::play() const {
        alCall(alSourcePlay, id);
        ALint state = AL_PLAYING;
        while (state == AL_PLAYING) {
            alCall(alGetSourcei, id, AL_SOURCE_STATE, &state);
        }
    }

    void Source::pause() const {
        alCall(alSourcePause, id);
    }

    void Source::stop() const {
        alCall(alSourceStop, id);
    }

    void Source::updateStream() {
        ALint buffersProcessed = 0;
        alCall(alGetSourcei, id, AL_BUFFERS_PROCESSED, &buffersProcessed);

        if (buffersProcessed <= 0) return;

//        ENGINE_INFO("Audio buffers processed {0}", buffersProcessed);

        size_t cursorValue = cursor.size();
        s32 bufferSize = (s32) cursor.bufferSize;
        s32 dataSize = format.size;

        ALint state = alCall(alGetSourcei, id, AL_SOURCE_STATE, &state);
        while (buffersProcessed-- && state == AL_PLAYING) {
            ALuint buffer;
            alCall(alSourceUnqueueBuffers, id, 1, &buffer);

            std::size_t dataSizeToCopy = bufferSize;
            if (cursorValue + bufferSize > dataSize) {
                dataSizeToCopy = dataSize - cursorValue;
            }

            cursorValue += dataSizeToCopy;

            if (dataSizeToCopy < bufferSize) {
                cursorValue = bufferSize - dataSizeToCopy;
            }

            char* data = io::AudioFile::streamWav((s64) (currentBufferIndex * bufferSize), (s64) bufferSize);
            io::AudioData audioSubData { { bufferSize, format.frequency, format.channels }, data };
            Buffer::load(buffer, audioSubData);
            delete[] data;

            alCall(alSourceQueueBuffers, id, 1, &buffer);

            currentBufferIndex++;
            if (currentBufferIndex >= cursor.bufferCount) {
                currentBufferIndex = 0;
            }

            state = alCall(alGetSourcei, id, AL_SOURCE_STATE, &state);
        }
    }

    void Source::playStream() {
        queueBuffers();
        alCall(alSourcePlay, id);

        ALint state = AL_PLAYING;
        while (state == AL_PLAYING) {
            updateStream();
            alCall(alGetSourcei, id, AL_SOURCE_STATE, &state);
        }
    }

    void Source::unQueueBuffers() const {
        Buffer buffer = buffers[0];
        alCall(alSourceUnqueueBuffers, id, buffers.size(), &buffer.getRef());
    }

    void Source::clearBuffers() {
        size_t bufferCount = buffers.size();
        u32* bufferIds = new u32[bufferCount];
        for (u32 i = 0; i < bufferCount; i++) {
            bufferIds[i] = buffers[i].get();
        }

        Buffer::destroy(bufferIds, bufferCount);
        buffers.clear();
    }

    void Source::setComponent(const AudioSourceComponent &component) const {
        setPosition(component.position);
        setVelocity(component.velocity);
        setGain(component.gain);
        setPitch(component.pitch);
        setLooping(component.looping);
        setRefDistance(component.refDistance);
        setMaxDistance(component.maxDistance);
        setRolloffFactor(component.rollOffFactor);
    }

    ALenum convertAttenuation(const Attenuation &attenuation) {
        switch (attenuation) {
            case Attenuation::NONE: return AL_NONE;
            case Attenuation::INVERSE_DISTANCE: return AL_INVERSE_DISTANCE;
            case Attenuation::INVERSE_DISTANCE_CLAMPED: return AL_INVERSE_DISTANCE_CLAMPED;
            case Attenuation::LINEAR_DISTANCE: return AL_LINEAR_DISTANCE;
            case Attenuation::LINEAR_DISTANCE_CLAMPED: return AL_LINEAR_DISTANCE_CLAMPED;
            case Attenuation::EXPONENT_DISTANCE: return AL_EXPONENT_DISTANCE;
            case Attenuation::EXPONENT_DISTANCE_CLAMPED: return AL_EXPONENT_DISTANCE_CLAMPED;
        }
    }

    void Source::setAttenuation(const Attenuation &attenuation) {
        alCall(alDistanceModel, convertAttenuation(attenuation));
    }

    void Source::setRefDistance(f32 refDistance) const {
        alCall(alSourcef, id, AL_REFERENCE_DISTANCE, refDistance);
    }

    void Source::setMaxDistance(f32 maxDistance) const {
        alCall(alSourcef, id, AL_MAX_DISTANCE, maxDistance);
    }

    void Source::setRolloffFactor(f32 rolloffFactor) const {
        alCall(alSourcef, id, AL_ROLLOFF_FACTOR, rolloffFactor);
    }

    void Source::queueBuffer(const u32 &bufferIndex) const {
        alCall(alSourceQueueBuffers, id, 1, &buffers[bufferIndex].get());
    }
}
