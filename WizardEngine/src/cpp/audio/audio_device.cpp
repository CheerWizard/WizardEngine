//
// Created by mecha on 22.04.2022.
//

#include <audio/audio_device.h>

namespace engine::audio {

    u8 Device::IDS = 0;

    Device::Device(const char* name) {
        handle = alcOpenDevice(name);
        if (!handle) {
            ENGINE_THROW(audio_device_exception("Failed to open audio device!"));
        }
        ENGINE_INFO("Audio device {0} opened!", name ? name : "DEFAULT");
    }

    Device::~Device() {
        ALCboolean closed;
        if (!alcCall(alcCloseDevice, closed, alHandle(ALCdevice), alHandle(ALCdevice))) {
            ENGINE_ERR("Failed when closing audio device");
        }
        ENGINE_INFO("Audio device closed!");

        if (!alcMakeContextCurrent(nullptr)) {
            ENGINE_ERR("Failed to set current audio context to NULL!");
        }
        ENGINE_INFO("Current audio context set to NULL!");
    }
}