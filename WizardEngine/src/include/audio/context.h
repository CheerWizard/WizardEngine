//
// Created by mecha on 22.04.2022.
//

#pragma once

#include <audio/device.h>

namespace engine::audio {

    decl_exception(audio_context_exception)

    class Context final {

    public:
        Context(Device* device);
        ~Context();

    private:
        void* handle;
        Device* device;
    };

}
