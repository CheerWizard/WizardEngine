//
// Created by mecha on 22.04.2022.
//

#pragma once

#include <core/vector.h>
#include <core/Memory.h>
#include <core/primitives.h>
#include <core/core.h>

#include <io/AudioFile.h>

namespace engine::audio {

    using namespace core;

    class ENGINE_API Buffer final {

    public:
        Buffer() = default;
        Buffer(const u32& id) : id(id) {}
        ~Buffer() = default;

    public:
        void create();
        static void create(u32* ids, u8 count);
        void destroy();
        static void destroy(u32* ids, u8 count);
        void recreate();

        void load(const io::AudioData &audioData) const;
        static void load(const u32& id, const io::AudioData &audioData);

    public:
        [[nodiscard]] inline const u32& get() const {
            return id;
        }

        [[nodiscard]] inline u32& getRef() {
            return id;
        }

        [[nodiscard]] inline u32 getValue() const {
            return id;
        }

    private:
        u32 id = 0;
    };

}
