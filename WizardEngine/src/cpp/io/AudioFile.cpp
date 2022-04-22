//
// Created by mecha on 22.04.2022.
//

#include <io/AudioFile.h>
#include <io/Logger.h>

namespace engine::io {

    s32 AudioFile::toInt(char* buffer, size_t length) {
        s32 result = 0;

        if (endian::native == endian::little) {
            std::memcpy(&result, buffer, length);
        } else {
            for(size_t i = 0; i < length; ++i) {
                reinterpret_cast<char*>(&result)[3 - i] = buffer[i];
            }
        }

        return result;
    }

    bool AudioFile::readWavFileHeader(
            std::ifstream& file,
            u8& channels,
            s32& sampleRate,
            u8& bitsPerSample,
            s32& size
    ) {
        char buffer[4];
        if (!file.is_open()) {
            return false;
        }

        // the RIFF
        if (!file.read(buffer, 4)) {
            ENGINE_ERR("WavFile: could not read RIFF");
            return false;
        }

        if (std::strncmp(buffer, "RIFF", 4) != 0) {
            ENGINE_ERR("WavFile: file is not a valid WAVE file (header doesn't begin with RIFF)");
            return false;
        }

        // the size of the file
        if (!file.read(buffer, 4)) {
            ENGINE_ERR("WavFile: fcould not read size of file");
            return false;
        }

        // the WAVE
        if (!file.read(buffer, 4)) {
            ENGINE_ERR("WavFile: could not read WAVE");
            return false;
        }

        if (std::strncmp(buffer, "WAVE", 4) != 0) {
            ENGINE_ERR("WavFile: file is not a valid WAVE file (header doesn't contain WAVE)");
            return false;
        }

        // "fmt/0"
        if (!file.read(buffer, 4)) {
            ENGINE_ERR("WavFile: could not read fmt/0");
            return false;
        }

        // this is always 16, the size of the fmt data chunk
        if (!file.read(buffer, 4)) {
            ENGINE_ERR("WavFile: could not read the 16");
            return false;
        }

        // PCM should be 1?
        if (!file.read(buffer, 2)) {
            ENGINE_ERR("WavFile: could not read PCM");
            return false;
        }

        // the number of channels
        if (!file.read(buffer, 2)) {
            ENGINE_ERR("WavFile: could not read number of channels");
            return false;
        }

        channels = toInt(buffer, 2);

        // sample rate
        if (!file.read(buffer, 4)) {
            ENGINE_ERR("WavFile: could not read sample rate");
            return false;
        }
        sampleRate = toInt(buffer, 4);

        // (sampleRate * bitsPerSample * channels) / 8
        if (!file.read(buffer, 4)) {
            ENGINE_ERR("WavFile: could not read (sampleRate * bitsPerSample * channels) / 8");
            return false;
        }

        // ?? dafaq
        if (!file.read(buffer, 2)) {
            ENGINE_ERR("WavFile: could not read dafaq");
            return false;
        }

        // bitsPerSample
        if (!file.read(buffer, 2)) {
            ENGINE_ERR("WavFile: could not read bits per sample");
            return false;
        }
        bitsPerSample = toInt(buffer, 2);

        // data chunk header "data"
        if (!file.read(buffer, 4)) {
            ENGINE_ERR("WavFile: could not read data chunk header");
            return false;
        }

        if (std::strncmp(buffer, "data", 4) != 0) {
            ENGINE_ERR("WavFile: file is not a valid WAVE file (doesn't have 'data' tag)");
            return false;
        }

        // size of data
        if (!file.read(buffer, 4)) {
            ENGINE_ERR("WavFile: could not read data size");
            return false;
        }
        size = toInt(buffer, 4);

        /* cannot be at the end of file */
        if (file.eof()) {
            ENGINE_ERR("WavFile: reached EOF on the file");
            return false;
        }

        if (file.fail()) {
            ENGINE_ERR("WavFile: fail state set on the file");
            return false;
        }

        return true;
    }

    AudioData AudioFile::readWav(const char* filePath) {
        std::ifstream in(filePath, std::ios::binary);

        u8 channels;
        s32 sampleRate;
        u8 bitsPerSample;
        s32 size;

        if (!in.is_open()) {
            ENGINE_ERR("Unable to open file {0}", filePath);
            ENGINE_THROW(file_not_found(filePath));
        }

        if(!readWavFileHeader(in, channels, sampleRate, bitsPerSample, size)) {
            ENGINE_ERR("Unable to load wav header of file {0}", filePath);
            ENGINE_THROW(file_not_found(filePath));
        }

        char* data = new char[size];
        in.read(data, size);

        in.close();

        return { data, 0, size, sampleRate, 0 };
    }

}