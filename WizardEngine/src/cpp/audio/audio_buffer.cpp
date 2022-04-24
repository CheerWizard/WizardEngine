//
// Created by mecha on 22.04.2022.
//

#include <audio/audio_buffer.h>
#include <audio/audio_core.h>

namespace engine::audio {

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

    u32 convertChannels(const io::Channels& channels) {
        switch (channels) {
            case io::Channels::MONO_8: return AL_FORMAT_MONO8;
            case io::Channels::MONO_16: return AL_FORMAT_MONO16;
            case io::Channels::STEREO_8: return AL_FORMAT_STEREO8;
            case io::Channels::STEREO_16: return AL_FORMAT_STEREO16;
        }
    }

    void Buffer::load(const io::AudioData &audioData) const {
        ENGINE_INFO("Audio buffer[id={0}] load()", id);
        alCall(alBufferData, id, convertChannels(audioData.format.channels),
               audioData.data, audioData.format.size, audioData.format.frequency);
    }

    void Buffer::load(const u32 &id, const io::AudioData &audioData) {
        ENGINE_INFO("Audio buffer[id={0}] load()", id);
        alCall(alBufferData, id, convertChannels(audioData.format.channels),
               audioData.data, audioData.format.size, audioData.format.frequency);
    }
}