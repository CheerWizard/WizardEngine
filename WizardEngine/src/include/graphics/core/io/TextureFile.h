//
// Created by mecha on 10.11.2021.
//

#pragma once

#include <core/filesystem.h>
#include <graphics/core/texture/Texture.h>

#define RUNTIME_TEXTURES_PATH "assets/textures"

#define CHANNEL_RED 1
#define CHANNEL_RGB 3
#define CHANNEL_RGBA 4

namespace engine::io {

    using namespace graphics;

    enum Spectrum {
        NONE, SRGB, HDR,
    };

    struct ENGINE_API TextureData {
        int width = 0, height = 0;
        Spectrum spectrum = NONE;
        int internalFormat = ColorFormat::RED;
        u32 dataFormat = ColorFormat::U_RED;
        u32 pixelsType = PixelsType::U_BYTE;
        void* pixels = nullptr;
    };

    struct ENGINE_API TextureArrayData {
        int width = 0, height = 0;
        Spectrum spectrum = NONE;
        int internalFormat = ColorFormat::RED;
        u32 dataFormat = ColorFormat::U_RED;
        u32 pixelsType = PixelsType::U_BYTE;
        vector<TextureData> textureData;
    };

    class ENGINE_API TextureFile final {

    private:
        TextureFile() = default;
        ~TextureFile() = default;

    public:
        static void setFlipTexture(bool flipEnabled);
        // returns NULL if read from file - failed
        static TextureData read(const char* filePath, Spectrum spectrum = NONE);
        static void free(void* data);
        static TextureArrayData read(const vector<std::string>& filepaths, Spectrum spectrum = NONE);
        static void free(const TextureArrayData& textureArrayData);
    };

}
