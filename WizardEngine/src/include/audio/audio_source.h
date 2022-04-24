//
// Created by mecha on 22.04.2022.
//

#pragma once

#include <audio/audio_buffer.h>
#include <glm/glm.hpp>
#include <ecs/ecs.h>

namespace engine::audio {

    using namespace core;

    struct Cursor {
        u8 bufferCount = 0;
        size_t bufferSize = 0;

        [[nodiscard]] inline size_t size() const {
            return bufferCount * bufferSize;
        }
    };

    component(AudioSourceComponent) {
        u32 sourceId = 0;
        glm::vec3 position = { 0, 0, 0 };
        glm::vec3 velocity = { 0, 0, 0 };
        f32 gain = 1;
        f32 pitch = 1;
        bool looping = false;
        f32 refDistance = 1;
        f32 maxDistance = max_f32;
        f32 rollOffFactor = 1;
    };

    enum Attenuation {
        NONE,
        INVERSE_DISTANCE, INVERSE_DISTANCE_CLAMPED,
        LINEAR_DISTANCE, LINEAR_DISTANCE_CLAMPED,
        EXPONENT_DISTANCE, EXPONENT_DISTANCE_CLAMPED,
        DEFAULT = INVERSE_DISTANCE_CLAMPED
    };

    class Source final {

    public:
        Source() = default;
        ~Source() = default;

    public:
        void create(const u8& bufferCount);
        void destroy();
        void recreate();

        void load(const io::AudioData& audioData);
        void loadStream(const io::AudioData& audioData, const Cursor& cursor);
        void updateStream() const;

        void setPosition(const glm::vec3& position) const;
        void setVelocity(const glm::vec3& velocity) const;
        void setPitch(f32 pitch) const;
        void setGain(f32 gain) const;
        void setLooping(bool enabled) const;
        void setRefDistance(f32 refDistance) const;
        void setMaxDistance(f32 maxDistance) const;
        void setRolloffFactor(f32 rolloffFactor) const;
        void setComponent(const AudioSourceComponent& component) const;

        static void setAttenuation(const Attenuation& attenuation);

        void setBuffer(const u32& bufferIndex) const;
        void queueBuffers() const;
        void unQueueBuffers() const;
        void clearBuffers();

        void play() const;
        void pause() const;
        void stop() const;

        void playStream() const;

    public:
        [[nodiscard]] inline const u32& get() const {
            return id;
        }

        inline void setCursor(const Cursor& inCursor) {
            cursor = inCursor;
        }

    private:
        u32 id = 0;
        vector<Buffer> buffers;
        Cursor cursor;
        io::AudioData audioData;
    };

}
