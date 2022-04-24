//
// Created by mecha on 22.04.2022.
//

#pragma once

#include <audio/buffer.h>
#include <glm/glm.hpp>

namespace engine::audio {

    using namespace core;

    struct Cursor {
        u8 bufferCount = 0;
        size_t bufferSize = 0;

        [[nodiscard]] inline size_t size() const {
            return bufferCount * bufferSize;
        }
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

        void setPitch(f32 pitch) const;
        void setGain(f32 gain) const;

        void setPosition(const glm::vec3& position) const;
        void setVelocity(const glm::vec3& velocity) const;

        void setLooping(bool enabled) const;
        void setBuffer(const u32& bufferIndex) const;
        void queueBuffers() const;

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
