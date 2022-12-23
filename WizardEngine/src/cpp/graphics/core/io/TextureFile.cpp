//
// Created by mecha on 10.11.2021.
//

#include <graphics/core/io/TextureFile.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <sstream>

namespace engine::io {

    TextureData TextureFile::read(const char* filePath, Spectrum spectrum) {
        ENGINE_INFO("Reading texture from '{0}'", filePath);
        int width, height, channels;
        void* pixels;

        if (spectrum == HDR) {
            pixels = stbi_loadf(filePath, &width, &height, &channels, 0);
        } else {
            pixels = stbi_load(filePath, &width, &height, &channels, 0);
        }

        if (!pixels) {
            if (stbi_failure_reason()) {
                ENGINE_ERR("Error occurs when loading texture from '{0}'", filePath);
                ENGINE_ERR("stbi failure reason : {0}", stbi_failure_reason());
            }
            return { width, height, spectrum };
        }

        TextureData textureData;
        textureData.width = width;
        textureData.height = height;
        textureData.pixels = pixels;

        switch (spectrum) {
            case HDR:
                switch (channels) {
                    case CHANNEL_RGB:
                        textureData.internalFormat = ColorFormat::RGB16F;
                        textureData.dataFormat = ColorFormat::RGB;
                        break;
                    case CHANNEL_RGBA:
                        textureData.internalFormat = ColorFormat::RGBA16F;
                        textureData.dataFormat = ColorFormat::RGBA;
                        break;
                    default:
                        textureData.internalFormat = ColorFormat::R16F;
                        textureData.dataFormat = ColorFormat::U_RED;
                        break;
                }
                textureData.pixelsType = PixelsType::FLOAT;
                break;
            case SRGB:
                switch (channels) {
                    case CHANNEL_RGB:
                        textureData.internalFormat = ColorFormat::SRGB;
                        textureData.dataFormat = ColorFormat::RGB;
                        break;
                    case CHANNEL_RGBA:
                        textureData.internalFormat = ColorFormat::SRGBA;
                        textureData.dataFormat = ColorFormat::RGBA;
                        break;
                    default:
                        textureData.internalFormat = ColorFormat::RED;
                        textureData.dataFormat = ColorFormat::U_RED;
                        break;
                }
                textureData.pixelsType = PixelsType::U_BYTE;
                break;
            default:
                switch (channels) {
                    case CHANNEL_RGB:
                        textureData.internalFormat = ColorFormat::RGB8;
                        textureData.dataFormat = ColorFormat::RGB;
                        break;
                    case CHANNEL_RGBA:
                        textureData.internalFormat = ColorFormat::RGBA8;
                        textureData.dataFormat = ColorFormat::RGBA;
                        break;
                    default:
                        textureData.internalFormat = ColorFormat::RED;
                        textureData.dataFormat = ColorFormat::U_RED;
                        break;
                }
                textureData.pixelsType = PixelsType::U_BYTE;
                break;
        }

        return textureData;
    }

    void TextureFile::free(void *pixels) {
        stbi_image_free(pixels);
    }

    TextureArrayData TextureFile::read(const vector<std::string> &filepaths, Spectrum spectrum) {
        TextureArrayData textureArrayData;
        for (const auto& filepath : filepaths) {
            TextureData textureData = read(filepath.c_str());
            if (textureData.pixels) {
                textureArrayData.textureData.emplace_back(textureData);
            }
        }

        if (!textureArrayData.textureData.empty()) {
            TextureData& textureData = textureArrayData.textureData[0];
            textureArrayData.width = textureData.width;
            textureArrayData.height = textureData.height;
            textureArrayData.spectrum = spectrum;
            textureArrayData.internalFormat = textureData.internalFormat;
            textureArrayData.dataFormat = textureData.dataFormat;
            textureArrayData.pixelsType = textureData.pixelsType;
        }

        return textureArrayData;
    }

    void TextureFile::free(const TextureArrayData &textureArrayData) {
        for (const auto& textureData : textureArrayData.textureData) {
            free(textureData.pixels);
        }
    }

    void TextureFile::setFlipTexture(bool flipEnabled) {
        stbi_set_flip_vertically_on_load(flipEnabled);
    }

    void TextureFile::free(const TextureData &textureData) {
        free(textureData.pixels);
    }

}