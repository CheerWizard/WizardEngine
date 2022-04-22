//
// Created by mecha on 22.04.2022.
//

#include <audio/Device.h>
#include <AL/al.h>

namespace engine::audio {

    Ref<Device>& Device::get() {
        static Ref<Device> instance = createRef<Device>();
        return instance;
    }

    Device::Device() {
        device = alcOpenDevice(nullptr);
        if (!device) {
            ENGINE_THROW(audio_device_exception("Failed to open audio device!"));
        }

        context = alcCreateContext(device, nullptr);
        if (!context) {
            ENGINE_THROW(audio_device_exception("Failed to create audio context!"));
        }

        if (!alcMakeContextCurrent(context)) {
            ENGINE_THROW(audio_device_exception("Failed to make context as current!"));
        }

        const ALchar* name = nullptr;
        if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT")) {
            name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
        }
        if (!name || alcGetError(device) != AL_NO_ERROR) {
            name = alcGetString(device, ALC_DEVICE_SPECIFIER);
        }

        ENGINE_INFO("Audio: device {0} opened!", name);
    }

    Device::~Device() {
        if (!alcMakeContextCurrent(nullptr)) {
            ENGINE_THROW(audio_device_exception("Failed to set current audio context to NULL!"));
        }

        alcDestroyContext(context);
        if (context) {
            ENGINE_THROW(audio_device_exception("Failed to destroy audio context!"));
        }

        if (!alcCloseDevice(device)) {
            ENGINE_THROW(audio_device_exception("Failed to close audio device!"));
        }
    }

}