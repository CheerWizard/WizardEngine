//
// Created by mecha on 10.11.2021.
//

#include <graphics/core/io/TextureFile.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <sstream>

namespace engine::io {

    TextureData TextureFile::read(const char* filePath, bool hdrEnabled) {
        ENGINE_INFO("Reading texture from '{0}'", filePath);
        int width, height, channels;
        void* data;
        if (hdrEnabled) {
            data = stbi_loadf(filePath, &width, &height, &channels, 0);
        } else {
            data = stbi_load(filePath, &width, &height, &channels, 0);
        }

        if (data == nullptr) {
            if (stbi_failure_reason()) {
                ENGINE_ERR("Error occurs when loading texture from '{0}'", filePath);
                ENGINE_ERR("stbi failure reason : {0}", stbi_failure_reason());
            }
            return { width, height, channels };
        }

        return { width, height, channels, false, hdrEnabled, data };
    }

    void TextureFile::free(void *data) {
        stbi_image_free(data);
    }

    TextureArrayData TextureFile::read(const vector<std::string> &filepaths) {
        TextureArrayData textureArrayData;
        for (const auto& filepath : filepaths) {
            TextureData textureData = read(filepath.c_str());
            if (textureData.data != nullptr) {
                textureArrayData.textureData.emplace_back(textureData);
            }
        }

        if (!textureArrayData.textureData.empty()) {
            TextureData& textureData = textureArrayData.textureData[0];
            textureArrayData.width = textureData.width;
            textureArrayData.height = textureData.height;
            textureArrayData.channels = textureData.channels;
        }

        return textureArrayData;
    }

    void TextureFile::free(const TextureArrayData &textureArrayData) {
        for (const auto& textureData : textureArrayData.textureData) {
            free(textureData.data);
        }
    }

    void TextureFile::setFlipTexture(bool flipEnabled) {
        stbi_set_flip_vertically_on_load(flipEnabled);
    }

}