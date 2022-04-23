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
    typedef thread::VoidTask<const std::string&, const SourceLoaded&> LoadTask;
    typedef thread::VoidTask<const Source&> SourceTask;

    class MediaPlayer final {

    private:
        MediaPlayer() = default;
        ~MediaPlayer() = default;

    public:
        static void load(
                const std::string& filepath,
                const SourceLoaded& sourceLoaded,
                const std::function<void()> done
        );
        static void play(const Source& source);
        static void pause(const Source& source);
        static void stop(const Source& source);

    private:
        static void loadImpl(const std::string& filepath, const SourceLoaded& sourceLoaded);
        static void playImpl(const Source& source);
        static void pauseImpl(const Source& source);
        static void stopImpl(const Source& source);

    private:
        static LoadTask loadTask;
        static SourceTask playTask;
        static SourceTask manageTask;

        static unordered_map<u32, Source> sources;
    };

}
