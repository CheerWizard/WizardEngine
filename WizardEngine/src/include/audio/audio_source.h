//
// Created by mecha on 22.04.2022.
//

#pragma once

#include <audio/audio_buffer.h>
#include <ecs/ecs.h>

namespace engine::audio {

    using namespace core;

    struct ENGINE_API Cursor final {
        u8 bufferCount = 0;
        size_t bufferSize = 0;

        [[nodiscard]] inline size_t size() const {
            return bufferCount * bufferSize;
        }
    };

    struct ENGINE_API Attenuation final {
        static int NONE;
        static int INVERSE_DISTANCE;
        static int INVERSE_DISTANCE_CLAMPED;
        static int LINEAR_DISTANCE;
        static int LINEAR_DISTANCE_CLAMPED;
        static int EXPONENT_DISTANCE;
        static int EXPONENT_DISTANCE_CLAMPED;
        static int DEFAULT;
    };

    component(AudioSourceComponent) {
        serializable()
        u32 sourceId = 0;
        math::vec3f position = { 0, 0, 0 };
        math::vec3f velocity = { 0, 0, 0 };
        f32 gain = 1;
        f32 pitch = 1;
        bool looping = false;
        f32 refDistance = 1;
        f32 maxDistance = max_f32;
        f32 rollOffFactor = 1;
        int attenuation = Attenuation::DEFAULT;
    };

    class ENGINE_API Source final {

    public:
        Source() = default;
        ~Source() = default;

    public:
        void create(const u8& bufferCount);
        void destroy();
        void recreate();

        void load(const AudioData& audioData);
        void load(const u32& bufferIndex, const AudioData& audioData);
        void loadStream(const std::string& filepath);
        void updateStream();

        void setPosition(const math::vec3f& position) const;
        void setVelocity(const math::vec3f& velocity) const;
        void setPitch(f32 pitch) const;
        void setGain(f32 gain) const;
        void setLooping(bool enabled) const;
        void setRefDistance(f32 refDistance) const;
        void setMaxDistance(f32 maxDistance) const;
        void setRolloffFactor(f32 rolloffFactor) const;
        void setComponent(const AudioSourceComponent& component) const;

        static void setAttenuation(int attenuation);

        void setBuffer(const u32& bufferIndex) const;
        void queueBuffers() const;
        void queueBuffer(const u32& bufferIndex) const;
        void unQueueBuffers() const;
        void clearBuffers();

        void play() const;
        void pause() const;
        void stop() const;

        void playStream();

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
        AudioFormat format;
        u8 currentBufferIndex = 0;
    };

}
