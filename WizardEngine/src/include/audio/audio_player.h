//
// Created by mecha on 22.04.2022.
//

#pragma once

#include <io/AudioFile.h>
#include <audio/audio_source.h>
#include <thread/Task.h>
#include <core/map.h>

namespace engine::audio {

    typedef std::function<void(const Source&)> SourceLoaded;
    typedef thread::VoidTask<const std::string&, const AudioSourceComponent&, const SourceLoaded&> LoadTask;
    typedef thread::VoidTask<const u32&> SourceTask;

    class ENGINE_API MediaPlayer final {

    private:
        MediaPlayer() = default;
        ~MediaPlayer() = default;

    public:
        static void load(
                const std::string& filepath,
                const AudioSourceComponent& audioSourceComponent,
                const SourceLoaded& sourceLoaded,
                const std::function<void()>& done
        );

        static void play(const Source& source);
        static void pause(const Source& source);
        static void stop(const Source& source);
        static void play();
        static void pause();
        static void stop();

        static void playStream(const Source& source);
        static void playStream();

        static void clear();

        static void loadStream(
                const std::string& filepath,
                const AudioSourceComponent& audioSourceComponent,
                const SourceLoaded& sourceLoaded,
                const std::function<void()>& done
        );

        static void setPlayedSource(const Source& source);

    private:
        static void loadImpl(
                const std::string& filepath,
                const AudioSourceComponent& audioSourceComponent,
                const SourceLoaded& sourceLoaded
        );
        static void playImpl(const u32& sourceId);
        static void pauseImpl(const u32& sourceId);
        static void stopImpl(const u32& sourceId);

        static void playStreamImpl(const u32& sourceId);

        static void loadStreamImpl(
                const std::string& filepath,
                const AudioSourceComponent& audioSourceComponent,
                const SourceLoaded& sourceLoaded
        );

    private:
        static LoadTask loadTask;
        static SourceTask playTask;
        static SourceTask manageTask;

        static unordered_map<u32, Source> sources;
        static u32 playedSourceId;
    };

}
