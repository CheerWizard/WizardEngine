//
// Created by mecha on 22.04.2022.
//

#include <audio/audio_player.h>

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
            const AudioSourceComponent& audioSourceComponent,
            const SourceLoaded& sourceLoaded,
            const std::function<void()>& done
    ) {
        loadTask.done = done;
        loadTask.run(filepath, audioSourceComponent, sourceLoaded);
    }

    void MediaPlayer::loadImpl(
            const std::string& filepath,
            const AudioSourceComponent& audioSourceComponent,
            const SourceLoaded& sourceLoaded
    ) {
        io::AudioData audioData = io::AudioFile::readWav(filepath.c_str());

        Source source;
        source.create(1);
        source.load(audioData);
        source.setComponent(audioSourceComponent);
        source.setBuffer(0);

        sources.insert(std::pair<u32, Source>(source.get(), source));

        sourceLoaded(source);
    }

    void MediaPlayer::play(const Source &source) {
        playTask.isRunning = false;
        playTask.runnable = playImpl;
        playTask.run(source.get());
    }

    void MediaPlayer::playImpl(const u32& sourceId) {
        playedSourceId = sourceId;
        const auto& source = sources.at(sourceId);
        source.stop();
        source.play();
    }

    void MediaPlayer::pause(const Source &source) {
        manageTask.runnable = pauseImpl;
        manageTask.run(source.get());
    }

    void MediaPlayer::pauseImpl(const u32& sourceId) {
        sources.at(sourceId).pause();
    }

    void MediaPlayer::stop(const Source &source) {
        manageTask.runnable = stopImpl;
        manageTask.run(source.get());
    }

    void MediaPlayer::stopImpl(const u32& sourceId) {
        sources.at(sourceId).stop();
    }

    void MediaPlayer::loadStream(
            const std::string &filepath,
            const AudioSourceComponent& audioSourceComponent,
            const SourceLoaded &sourceLoaded,
            const std::function<void()> &done
    ) {
        loadTask.runnable = loadStreamImpl;
        loadTask.done = done;
        loadTask.run(filepath, audioSourceComponent, sourceLoaded);
    }

    void MediaPlayer::loadStreamImpl(
            const std::string &filepath,
            const AudioSourceComponent& audioSourceComponent,
            const SourceLoaded &sourceLoaded
    ) {
        io::AudioData audioData = io::AudioFile::readWav(filepath.c_str());
        Cursor cursor {
            static_cast<u8>(audioData.size / kb_512),
            kb_512
        };

        Source source;
        source.create(cursor.bufferCount);
        source.loadStream(audioData, cursor);
        source.setComponent(audioSourceComponent);

        sources.insert(std::pair<u32, Source>(source.get(), source));

        sourceLoaded(source);
    }

    void MediaPlayer::clear() {
        stopImpl(playedSourceId);
        for (auto source : sources) {
            source.second.destroy();
        }
        sources.clear();
    }

    void MediaPlayer::play() {
        playTask.isRunning = false;
        playTask.runnable = playImpl;
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

    void MediaPlayer::setPlayedSource(const Source &source) {
        playedSourceId = source.get();
    }

    void MediaPlayer::playStream(const Source &source) {
        playTask.isRunning = false;
        playTask.runnable = playStreamImpl;
        playTask.run(source.get());
    }

    void MediaPlayer::playStreamImpl(const u32 &sourceId) {
        playedSourceId = sourceId;
        const auto& source = sources.at(sourceId);
        source.stop();
        source.playStream();
    }

    void MediaPlayer::playStream() {
        playTask.isRunning = false;
        playTask.runnable = playStreamImpl;
        playTask.run(playedSourceId);
    }
}