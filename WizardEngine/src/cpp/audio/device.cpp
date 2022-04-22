//
// Created by mecha on 22.04.2022.
//

#include <audio/device.h>

namespace engine::audio {

    Device::Device(const char* name) {
        handle = alcOpenDevice(name);
        if (!handle) {
            ENGINE_THROW(audio_device_exception("Failed to open audio device!"));
        }
        ENGINE_INFO("Audio device {0} opened!", name);
    }

    Device::~Device() {
        ALCboolean closed;
        if (!alcCall(alcCloseDevice, closed, alHandle(ALCdevice), alHandle(ALCdevice))) {
            ENGINE_ERR("Failed when closing audio device");
        }

        if (!alcMakeContextCurrent(nullptr)) {
            ENGINE_ERR("Failed to set current audio context to NULL!");
        }
    }

    vector<std::string> Device::getAvailableDevices() {
        const ALCchar* devices;
        if (!alcCall(alcGetString, devices, alHandle(ALCdevice), nullptr, ALC_DEVICE_SPECIFIER)) {
            ENGINE_THROW(audio_device_exception("No available devices!"));
        }

        const char* ptr = devices;
        vector<std::string> devicesVec;

        do {
            devicesVec.push_back(std::string(ptr));
            ptr += devicesVec.back().size() + 1;
        } while(*(ptr + 1) != '\0');

        return devicesVec;
    }

}