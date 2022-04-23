//
// Created by mecha on 22.04.2022.
//

#include <audio/context.h>

namespace engine::audio {

    Context::Context(const Ref<Device>& device) : device(device) {
        auto* deviceHandle = (ALCdevice*) device->get();
        if (!alcCall(alcCreateContext, handle, deviceHandle, deviceHandle, nullptr) || !handle) {
            ENGINE_THROW(audio_context_exception("Failed to create audio context!"));
        }
        ENGINE_INFO("Audio context created!");

        ALCboolean contextMadeCurrent = false;
        if (!alcCall(alcMakeContextCurrent, contextMadeCurrent, deviceHandle, alHandle(ALCcontext))
        || contextMadeCurrent != ALC_TRUE) {
            ENGINE_THROW(audio_context_exception("Failed to make audio context current"));
        }
        ENGINE_INFO("Audio context set to current!");
    }

    Context::~Context() {
        auto* deviceHandle = (ALCdevice*) device->get();

        ALCboolean contextMadeCurrent = false;
        if (!alcCall(alcMakeContextCurrent, contextMadeCurrent, deviceHandle, nullptr)) {
            ENGINE_ERR("Failed to remove current audio context!");
        }
        ENGINE_INFO("Current audio context removed!");

        if (!alcCall(alcDestroyContext, deviceHandle, alHandle(ALCcontext))) {
            ENGINE_ERR("Failed to destroy audio context!");
        }
        ENGINE_INFO("Audio context destroyed!");

        device = nullptr;
    }

}