//
// Created by mecha on 22.04.2022.
//

#pragma once

#include <audio/audio_core.h>
#include <io/Logger.h>

namespace engine::audio {

    using namespace core;

    decl_exception(audio_device_exception);

    class ENGINE_API Device final {

    public:
        Device(const char* name = nullptr);
        ~Device();

    public:
        inline void* get() {
            return handle;
        }

        [[nodiscard]] inline u8 getId() const {
            return id;
        }

    private:
        static u8 IDS;
        u8 id = IDS++;
        void* handle;
    };
}
