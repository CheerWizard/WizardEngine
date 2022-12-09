//
// Created by mecha on 22.04.2022.
//

#pragma once

#include <core/core.h>
#include <core/vector.h>
#include <core/Memory.h>
#include <core/primitives.h>
#include <core/exception.h>

namespace engine::audio {

    using namespace core;

    decl_exception(audio_format_exception)

    struct ENGINE_API Channels final {
        static int MONO_8;
        static int MONO_16;
        static int STEREO_8;
        static int STEREO_16;
        static int DEFAULT;
    };

    struct ENGINE_API AudioFormat {
        s32 size = 0;
        s32 frequency = 0;
        int channels = Channels::DEFAULT;
    };

    struct ENGINE_API AudioData {
        AudioFormat format;
        char* data = nullptr;
    };

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

        void load(const AudioData &audioData) const;
        static void load(const u32& id, const AudioData &audioData);

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
