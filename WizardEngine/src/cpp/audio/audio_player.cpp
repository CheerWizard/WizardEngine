//
// Created by mecha on 22.04.2022.
//

#include <audio/audio_player.h>
#include <audio/devices.h>

namespace engine::audio {

    LoadTask MediaPlayer::loadTask = {
            "MediaPlayer_LoadTask",
            "MediaPlayer_LoadThread",
            loadImpl
    };

    SourceTask MediaPlayer::playTask = {
            "MediaPlayer_PlayTask",
            "MediaPlayer_PlayThread",
            playImpl
    };

    SourceTask MediaPlayer::manageTask = {
            "MediaPlayer_ManageTask",
            "MediaPlayer_ManageThread",
            stopImpl
    };

    unordered_map<u32, Source> MediaPlayer::sources;

    void MediaPlayer::load(
            const std::string& filepath,
            const SourceLoaded& sourceLoaded,
            const std::function<void()> done
    ) {
        loadTask.done = done;
        loadTask.run(filepath, sourceLoaded);
    }

    void MediaPlayer::loadImpl(const std::string& filepath, const SourceLoaded& sourceLoaded) {
        io::AudioData audioData = io::AudioFile::readWav(filepath.c_str());

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

        sourceLoaded(source);
    }

    void MediaPlayer::play(const Source &source) {
        playTask.run(source);
    }

    void MediaPlayer::playImpl(const Source &source) {
        alCall(alSourcePlay, source.get());
        ALint state = AL_PLAYING;
        while (state == AL_PLAYING) {
            alCall(alGetSourcei, source.get(), AL_SOURCE_STATE, &state);
        }
    }

    void MediaPlayer::pause(const Source &source) {
        manageTask.runnable = pauseImpl;
        manageTask.run(source);
    }

    void MediaPlayer::pauseImpl(const Source &source) {
        alCall(alSourcePause, source.get());
    }

    void MediaPlayer::stop(const Source &source) {
        manageTask.runnable = stopImpl;
        manageTask.run(source);
    }

    void MediaPlayer::stopImpl(const Source &source) {
        alCall(alSourceStop, source.get());
    }
}