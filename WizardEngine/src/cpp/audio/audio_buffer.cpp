//
// Created by mecha on 22.04.2022.
//

#include <audio/audio_buffer.h>
#include <audio/audio_core.h>
#include <io/Logger.h>

namespace engine::audio {

    int Channels::MONO_8 = AL_FORMAT_MONO8;
    int Channels::MONO_16 = AL_FORMAT_MONO16;
    int Channels::STEREO_8 = AL_FORMAT_STEREO8;
    int Channels::STEREO_16 = AL_FORMAT_STEREO16;
    int Channels::DEFAULT = MONO_8;

    void Buffer::create() {
        alCall(alGenBuffers, 1, &id);
    }

    void Buffer::create(u32 *ids, u8 count) {
        alCall(alGenBuffers, count, &ids[0]);
    }

    void Buffer::destroy() {
        alCall(alDeleteBuffers, 1, &id);
    }

    void Buffer::destroy(u32 *ids, u8 count) {
        alCall(alDeleteBuffers, count, &ids[0]);
    }

    void Buffer::recreate() {
        destroy();
        create();
    }

    void Buffer::load(const AudioData &audioData) const {
        ENGINE_INFO("Audio buffer[id={0}] load()", id);
        alCall(alBufferData, id, audioData.format.channels,
               audioData.data, audioData.format.size, audioData.format.frequency);
    }

    void Buffer::load(const u32 &id, const AudioData &audioData) {
        ENGINE_INFO("Audio buffer[id={0}] load()", id);
        alCall(alBufferData, id, audioData.format.channels,
               audioData.data, audioData.format.size, audioData.format.frequency);
    }
}