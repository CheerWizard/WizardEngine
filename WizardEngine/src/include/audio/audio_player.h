//
// Created by mecha on 22.04.2022.
//

#pragma once

#include <io/AudioFile.h>
#include <audio/audio_source.h>
#include <audio/context.h>

namespace engine::audio {

    class MediaPlayer final {

    private:
        MediaPlayer() = default;
        ~MediaPlayer() = default;

    public:
        static Source load(const char* filepath);
        static void play(const Source& source);
        static void pause(const Source& source);
        static void stop(const Source& source);
        static void destroy();

    private:
        static void initContext();

    private:
        static Device* device;
        static Context* context;

    };

}
