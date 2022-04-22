//
// Created by mecha on 22.04.2022.
//

#pragma once

#include <io/Logger.h>
#include <AL/alc.h>

namespace engine::audio {

    using namespace core;

    decl_exception(audio_device_exception);

    class Device {

    private:
        Device();
        ~Device();

    public:
        static Ref<Device>& get();

    private:
        ALCdevice* device;
        ALCcontext* context;
    };

}
