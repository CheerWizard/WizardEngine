//
// Created by mecha on 22.04.2022.
//

#pragma once

#include <audio/audio_core.h>
#include <io/Logger.h>

namespace engine::audio {

    using namespace core;

    decl_exception(audio_device_exception);

    class Device final {

    public:
        Device(const char* name = nullptr);
        ~Device();

    public:
        vector<std::string> getAvailableDevices();

        inline void* get() {
            return handle;
        }

    private:
        void* handle;
    };

}
