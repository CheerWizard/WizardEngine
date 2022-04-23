//
// Created by mecha on 22.04.2022.
//

#pragma once

#include <core/primitives.h>
#include <fstream>
#include <sstream>
#include <io/Logger.h>

namespace engine::io {

    using namespace core;

    decl_exception(audio_format_exception)

    enum AudioFormat {
        MONO_8, MONO_16,
        STEREO_8, STEREO_16,
        DEFAULT = MONO_8
    };

    struct AudioData {
        char* data = nullptr;
        u32 start = 0;
        s32 size = 0;
        s32 frequency = 0;
        AudioFormat format = DEFAULT;
    };

    class AudioFile final {

    private:
        AudioFile() = default;
        ~AudioFile() = default;

    public:
        static bool readWavFileHeader(
                std::ifstream& file,
                u8& channels,
                s32& sampleRate,
                u8& bitsPerSample,
                s32& size
        );
        static AudioData readWav(const char* filePath);

    private:
        static s32 toInt(char* buffer, size_t length);
    };

}