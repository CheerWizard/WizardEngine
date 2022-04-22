//
// Created by mecha on 22.04.2022.
//

#include <audio/buffer.h>
#include <audio/audio_core.h>

namespace engine::audio {

    void Buffer::create() {
        alCall(alGenBuffers, 1, &id);
    }

    void Buffer::destroy() {
        alCall(alDeleteBuffers, 1, &id);
    }

    void Buffer::recreate() {
        destroy();
        create();
    }

    void Buffer::load(const io::AudioData &audioData) const {
        alCall(alBufferData, id, audioData.format, audioData.data, audioData.size, audioData.frequency);
    }

}