//
// Created by mecha on 22.04.2022.
//

#pragma once

#include <audio/buffer.h>
#include <glm/glm.hpp>

namespace engine::audio {

    using namespace core;

    class Source final {

    public:
        Source() = default;
        ~Source() = default;

    public:
        void create();
        void destroy();
        void recreate();

        void load(const io::AudioData& audioData);

        void setPitch(f32 pitch) const;
        void setGain(f32 gain) const;

        void setPosition(const glm::vec3& position) const;
        void setVelocity(const glm::vec3& velocity) const;

        void setLooping(bool enabled) const;
        void setBuffer() const;

    public:
        [[nodiscard]] inline const u32& get() const {
            return id;
        }

    private:
        u32 id = 0;
        Buffer buffer;
        vector<Buffer> streamBuffers;
    };

}
