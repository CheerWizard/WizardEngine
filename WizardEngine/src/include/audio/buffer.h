//
// Created by mecha on 22.04.2022.
//

#pragma once

#include <core/vector.h>
#include <core/Memory.h>
#include <core/primitives.h>

#include <io/AudioFile.h>

#include <AL/al.h>

namespace engine::audio {

    using namespace core;

    class Buffer final {

    public:
        Buffer() = default;
        Buffer(const u32& id) : id(id) {}
        ~Buffer() = default;

    public:
        void create();
        static void create(u32* ids, u8 count);
        void destroy();
        void recreate();

        void load(const io::AudioData &audioData) const;
        static void load(const u32& id, const io::AudioData &audioData);

    public:
        [[nodiscard]] inline const u32& get() const {
            return id;
        }

    private:
        u32 id = 0;
    };

}
