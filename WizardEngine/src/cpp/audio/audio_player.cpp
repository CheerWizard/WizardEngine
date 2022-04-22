//
// Created by mecha on 22.04.2022.
//

#include <audio/audio_player.h>
#include <audio/audio_core.h>

namespace engine::audio {

    Device* MediaPlayer::device = nullptr;
    Context* MediaPlayer::context = nullptr;

    void MediaPlayer::initContext() {
        if (!context) {
            if (!device) {
                device = new Device();
            }
            context = new Context(device);
        }
    }

    void MediaPlayer::play(const Source &source) {
        alCall(alSourcePlay, source.get());
        ALint state = AL_PLAYING;
        while (state == AL_PLAYING) {
            alCall(alGetSourcei, source.get(), AL_SOURCE_STATE, &state);
        }
    }

    void MediaPlayer::pause(const Source &source) {
        alCall(alSourcePause, source.get());
    }

    void MediaPlayer::stop(const Source &source) {
        alCall(alSourceStop, source.get());
    }

    Source MediaPlayer::load(const char *filepath) {
        initContext();

        io::AudioData audioData = io::AudioFile::readWav(filepath);

        Buffer buffer;
        buffer.create();
        buffer.load(audioData);

        Source source;
        source.create();
        source.setPitch(1);
        source.setGain(1);
        source.setPosition({0, 0, 0});
        source.setVelocity({0, 0, 0});
        source.setLooping(false);
        source.setBuffer(buffer.get());

        return source;
    }

    void MediaPlayer::destroy() {
        delete context;
        delete device;
    }

}