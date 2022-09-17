//
// Created by mecha on 22.04.2022.
//

#pragma once

#include <audio/audio_device.h>

namespace engine::audio {

    decl_exception(audio_context_exception)

    class ENGINE_API Context final {

    public:
        Context(const Ref<Device>& device);
        ~Context();

    private:
        void* handle;
        Ref<Device> device;
    };

}
