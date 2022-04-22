//
// Created by mecha on 22.04.2022.
//

#include <audio/context.h>

namespace engine::audio {

    Context::Context(Device *device) : device(device) {
        auto* deviceHandle = (ALCdevice*) device->get();
        if (!alcCall(alcCreateContext, handle, deviceHandle, deviceHandle, nullptr) || !handle) {
            ENGINE_THROW(audio_context_exception("Failed to create audio context!"));
        }

        ALCboolean contextMadeCurrent = false;
        if (!alcCall(alcMakeContextCurrent, contextMadeCurrent, deviceHandle, alHandle(ALCcontext))
        || contextMadeCurrent != ALC_TRUE) {
            ENGINE_THROW(audio_context_exception("Failed to make audio context current"));
        }
    }

    Context::~Context() {
        auto* deviceHandle = (ALCdevice*) device->get();

        ALCboolean contextMadeCurrent = false;
        if (!alcCall(alcMakeContextCurrent, contextMadeCurrent, deviceHandle, nullptr)) {
            ENGINE_ERR("Failed to remove current audio context!");
        }

        if (!alcCall(alcDestroyContext, deviceHandle, alHandle(ALCcontext))) {
            ENGINE_ERR("Failed to destroy audio context!");
        }

        device = nullptr;
    }

}