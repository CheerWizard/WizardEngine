//
// Created by mecha on 22.04.2022.
//

#include <io/AudioFile.h>

namespace engine::io {

    std::ifstream AudioFile::filestream;

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
            u8& channels,
            s32& sampleRate,
            u8& bitsPerSample,
            s32& size
    ) {
        char buffer[4];
        // the RIFF
        if (!filestream.read(buffer, 4)) {
            ENGINE_ERR("WavFile: could not read RIFF");
            return false;
        }
        if (std::strncmp(buffer, "RIFF", 4) != 0) {
            ENGINE_ERR("WavFile: file is not a valid WAVE file (header doesn't begin with RIFF)");
            return false;
        }

        // the size of the file
        if (!filestream.read(buffer, 4)) {
            ENGINE_ERR("WavFile: fcould not read size of file");
            return false;
        }

        // the WAVE
        if (!filestream.read(buffer, 4)) {
            ENGINE_ERR("WavFile: could not read WAVE");
            return false;
        }
        if (std::strncmp(buffer, "WAVE", 4) != 0) {
            ENGINE_ERR("WavFile: file is not a valid WAVE file (header doesn't contain WAVE)");
            return false;
        }

        // "fmt/0"
        if (!filestream.read(buffer, 4)) {
            ENGINE_ERR("WavFile: could not read fmt/0");
            return false;
        }

        // this is always 16, the size of the fmt data chunk
        if (!filestream.read(buffer, 4)) {
            ENGINE_ERR("WavFile: could not read the 16");
            return false;
        }

        // PCM should be 1?
        if (!filestream.read(buffer, 2)) {
            ENGINE_ERR("WavFile: could not read PCM");
            return false;
        }

        // the number of channels
        if (!filestream.read(buffer, 2)) {
            ENGINE_ERR("WavFile: could not read number of channels");
            return false;
        }

        channels = toInt(buffer, 2);

        // sample rate
        if (!filestream.read(buffer, 4)) {
            ENGINE_ERR("WavFile: could not read sample rate");
            return false;
        }
        sampleRate = toInt(buffer, 4);

        // (sampleRate * bitsPerSample * channels) / 8
        if (!filestream.read(buffer, 4)) {
            ENGINE_ERR("WavFile: could not read (sampleRate * bitsPerSample * channels) / 8");
            return false;
        }

        // ?? dafaq
        if (!filestream.read(buffer, 2)) {
            ENGINE_ERR("WavFile: could not read dafaq");
            return false;
        }

        // bitsPerSample
        if (!filestream.read(buffer, 2)) {
            ENGINE_ERR("WavFile: could not read bits per sample");
            return false;
        }
        bitsPerSample = toInt(buffer, 2);

        // data chunk header "data"
        if (!filestream.read(buffer, 4)) {
            ENGINE_ERR("WavFile: could not read data chunk header");
            return false;
        }

        if (std::strncmp(buffer, "data", 4) != 0) {
            ENGINE_ERR("WavFile: file is not a valid WAVE file (doesn't have 'data' tag)");
            return false;
        }

        // size of data
        if (!filestream.read(buffer, 4)) {
            ENGINE_ERR("WavFile: could not read data size");
            return false;
        }
        size = toInt(buffer, 4);

        /* cannot be at the end of file */
        if (filestream.eof()) {
            ENGINE_ERR("WavFile: reached EOF on the file");
            return false;
        }
        if (filestream.fail()) {
            ENGINE_ERR("WavFile: fail state set on the file");
            return false;
        }

        return true;
    }

    AudioData AudioFile::readWav(const char *filepath) {
        open(filepath);

        AudioData audioData = readWavHeaders(filepath);
        char* data = new char[audioData.size];
        filestream.read(data, audioData.size);
        audioData.data = data;

        close();

        return audioData;
    }

    AudioData AudioFile::readWavHeaders(const char *filepath) {
        if (!isOpen()) {
            ENGINE_ERR("Unable to read file {0}. File is not opened!", filepath);
            ENGINE_THROW(file_not_found(filepath));
        }

        u8 channels;
        s32 sampleRate;
        u8 bitsPerSample;
        s32 size;

        if (!readWavFileHeader(channels, sampleRate, bitsPerSample, size)) {
            ENGINE_ERR("Unable to load wav header of file {0}", filepath);
            ENGINE_THROW(file_not_found(filepath));
        }

        AudioFormat format;

        if (channels == 1 && bitsPerSample == 8) {
            format = MONO_8;
        } else if (channels == 1 && bitsPerSample == 16) {
            format = MONO_16;
        } else if (channels == 2 && bitsPerSample == 8) {
            format = STEREO_8;
        } else if (channels == 2 && bitsPerSample == 16) {
            format = STEREO_16;
        } else {
            std::stringstream ss;
            ss << "Unsupported format exception : channels=" << channels << ", bps=" << bitsPerSample;
            ENGINE_THROW(audio_format_exception(ss.str()));
        }

        return AudioData { nullptr, size, sampleRate, format };
    }

    void AudioFile::open(const char *filepath) {
        filestream.open(filepath, std::ios::binary);
    }

    void AudioFile::close() {
        filestream.close();
    }

    bool AudioFile::isOpen() {
        return filestream.is_open();
    }

    char* AudioFile::streamWav(const s64& dataOffset, const s64& dataSize) {
        if (!isOpen()) {
            ENGINE_THROW(file_not_found("Unable to stream file. File is not opened!"));
        }

        char* data = new char[dataSize];
        filestream.seekg(dataOffset);
        filestream.read(data, dataSize);

        return data;
    }

}