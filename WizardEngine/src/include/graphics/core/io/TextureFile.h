//
// Created by mecha on 10.11.2021.
//

#pragma once

#include <core/filesystem.h>

#define RUNTIME_TEXTURES_PATH "assets/textures"

namespace engine::io {

    struct ENGINE_API TextureData {
        int width = 0, height = 0, channels = 0;
        bool sRGB = false;
        bool hdr = false;
        void* data = nullptr;
    };

    struct ENGINE_API TextureArrayData {
        int width = 0, height = 0, channels = 0;
        vector<TextureData> textureData;
    };

    class ENGINE_API TextureFile final {

    private:
        TextureFile() = default;
        ~TextureFile() = default;

    public:
        // returns NULL if read from file - failed
        static TextureData read(const char* filePath, bool hdrEnabled = false);
        static void free(void* data);
        static TextureArrayData read(const vector<std::string>& filepaths);
        static void free(const TextureArrayData& textureArrayData);
    };

}
