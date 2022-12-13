//
// Created by mecha on 22.04.2022.
//

#pragma once

#include <core/primitives.h>
#include <fstream>
#include <sstream>
#include <io/Logger.h>
#include <audio/audio_buffer.h>

namespace engine::io {

    using namespace core;
    using namespace audio;

    class ENGINE_API AudioFile final {

    private:
        AudioFile() = default;
        ~AudioFile() = default;

    public:
        static AudioData readWav(const char* filepath);
        static char* streamWav(const s64& dataOffset, const s64& dataSize);
        static AudioFormat readWavHeaders(const char* filepath);

        static void open(const char* filepath);
        static bool isOpen();
        static void close();

    private:
        static bool readWavFileHeader(
                u8& channels,
                s32& sampleRate,
                u8& bitsPerSample,
                s32& size
        );

    private:
        static s32 toInt(char* buffer, size_t length);

    private:
        static std::ifstream filestream;
    };

}
