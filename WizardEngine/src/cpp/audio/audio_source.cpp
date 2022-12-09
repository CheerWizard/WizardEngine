//
// Created by mecha on 22.04.2022.
//

#include <audio/audio_source.h>
#include <audio/audio_core.h>
#include <io/AudioFile.h>

namespace engine::audio {

    using namespace math;

    int Attenuation::NONE = AL_NONE;
    int Attenuation::INVERSE_DISTANCE = AL_INVERSE_DISTANCE;
    int Attenuation::INVERSE_DISTANCE_CLAMPED = AL_INVERSE_DISTANCE_CLAMPED;
    int Attenuation::LINEAR_DISTANCE =  AL_LINEAR_DISTANCE;
    int Attenuation::LINEAR_DISTANCE_CLAMPED = AL_LINEAR_DISTANCE_CLAMPED;
    int Attenuation::EXPONENT_DISTANCE = AL_EXPONENT_DISTANCE;
    int Attenuation::EXPONENT_DISTANCE_CLAMPED = AL_EXPONENT_DISTANCE_CLAMPED;
    int Attenuation::DEFAULT = EXPONENT_DISTANCE_CLAMPED;

    void AudioSourceComponent::serialize(YAML::Emitter &out) {
        out << YAML::BeginMap;
        out << YAML::Key << "AudioSourceComponent";

        yaml::serialize(out, "sourceId", sourceId);
        yaml::serialize(out, "position", position);
        yaml::serialize(out, "velocity", velocity);
        yaml::serialize(out, "gain", gain);
        yaml::serialize(out, "pitch", pitch);
        yaml::serialize(out, "looping", looping);
        yaml::serialize(out, "refDistance", refDistance);
        yaml::serialize(out, "maxDistance", maxDistance);
        yaml::serialize(out, "rollOffFactor", rollOffFactor);

        out << YAML::EndMap;
    }

    void AudioSourceComponent::deserialize(const YAML::Node &parent) {
        auto root = parent["AudioSourceComponent"];

        if (root) {
            sourceId = root["sourceId"].as<u32>();
            position = root["position"].as<vec3f>();
            velocity = root["velocity"].as<vec3f>();
            gain = root["gain"].as<f32>();
            pitch = root["pitch"].as<f32>();
            looping = root["looping"].as<bool>();
            refDistance = root["refDistance"].as<f32>();
            maxDistance = root["maxDistance"].as<f32>();
            rollOffFactor = root["rollOffFactor"].as<f32>();
        }
    }

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

    void Source::setPosition(const vec3f &position) const {
        alCall(alSource3f, id, AL_POSITION, position.x(), position.y(), position.z());
    }

    void Source::setVelocity(const vec3f &velocity) const {
        alCall(alSource3f, id, AL_VELOCITY, velocity.x(), velocity.y(), velocity.z());
    }

    void Source::setLooping(bool enabled) const {
        alCall(alSourcei, id, AL_LOOPING, enabled);
    }

    void Source::load(const AudioData &audioData) {
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

    void Source::load(const u32 &bufferIndex, const AudioData &audioData) {
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
            AudioData subData {
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
            AudioData audioSubData {
                { bufferSize, format.frequency, format.channels },
                data
            };
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

    void Source::setAttenuation(int attenuation) {
        alCall(alDistanceModel, attenuation);
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
