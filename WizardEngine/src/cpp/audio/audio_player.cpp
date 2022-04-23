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
    u32 MediaPlayer::playedSourceId;

    void MediaPlayer::load(
            const std::string& filepath,
            const SourceLoaded& sourceLoaded,
            const std::function<void()>& done
    ) {
        loadTask.done = done;
        loadTask.run(filepath, sourceLoaded);
    }

    void MediaPlayer::loadImpl(const std::string& filepath, const SourceLoaded& sourceLoaded) {
        io::AudioData audioData = io::AudioFile::readWav(filepath.c_str());

        Source source;
        source.create();
        source.load(audioData);
        source.setPitch(1);
        source.setGain(1);
        source.setPosition({0, 0, 0});
        source.setVelocity({0, 0, 0});
        source.setLooping(false);
        source.setBuffer();

        sources.insert(std::pair<u32, Source>(source.get(), source));

        sourceLoaded(source);
    }

    void MediaPlayer::play(const Source &source) {
        playTask.run(source.get());
    }

    void MediaPlayer::playImpl(const u32& sourceId) {
        playedSourceId = sourceId;
        alCall(alSourcePlay, sourceId);
        ALint state = AL_PLAYING;
        while (state == AL_PLAYING) {
            alCall(alGetSourcei, sourceId, AL_SOURCE_STATE, &state);
        }
    }

    void MediaPlayer::pause(const Source &source) {
        manageTask.runnable = pauseImpl;
        manageTask.run(source.get());
    }

    void MediaPlayer::pauseImpl(const u32& sourceId) {
        alCall(alSourcePause, sourceId);
    }

    void MediaPlayer::stop(const Source &source) {
        manageTask.runnable = stopImpl;
        manageTask.run(source.get());
    }

    void MediaPlayer::stopImpl(const u32& sourceId) {
        alCall(alSourceStop, sourceId);
    }

    void MediaPlayer::updateStream(
            const std::string &filepath,
            const SourceLoaded &sourceLoaded,
            const std::function<void()> &done
    ) {

    }

    void MediaPlayer::updateStreamImpl(
            const std::string &filepath,
            const SourceLoaded &sourceLoaded
    ) {
        ALuint buffers[NUM_BUFFERS];
        alCall(alGenBuffers, NUM_BUFFERS, &buffers[0]);
    }

    void MediaPlayer::clear() {
        stopImpl(playedSourceId);
        for (auto source : sources) {
            source.second.destroy();
        }
        sources.clear();
    }

    void MediaPlayer::play() {
        playTask.run(playedSourceId);
    }

    void MediaPlayer::pause() {
        manageTask.runnable = pauseImpl;
        manageTask.run(playedSourceId);
    }

    void MediaPlayer::stop() {
        manageTask.runnable = stopImpl;
        manageTask.run(playedSourceId);
    }
}