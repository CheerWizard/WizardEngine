//
// Created by mecha on 10.11.2021.
//

#include <graphics/core/io/TextureFile.h>

#include "stb_image.h"
#include "sstream"

namespace engine {

    TextureData TextureFile::read(const std::string_view &fileName) {
        return read(fileName, RUNTIME_TEXTURES_PATH);
    }

    void TextureFile::free(void *data) {
        stbi_image_free(data);
    }

    TextureData TextureFile::read(const std::string_view &fileName, const std::string_view &texturesPath) {
        // creating a full path to texture
        std::stringstream ss;
        ss << texturesPath << "/" << fileName;
        auto fullPath = ss.str();

        int width, height, channels;

        ENGINE_INFO("Reading texture from '{0}'", fullPath);
        void* data = stbi_load(fullPath.c_str(), &width, &height, &channels, 0);

        if (data == nullptr) {
            if (stbi_failure_reason()) {
                ENGINE_ERR("Error occurs when loading texture from '{0}'", fullPath);
                ENGINE_ERR("stbi failure reason : {0}", stbi_failure_reason());
            }
            return { width, height, channels };
        }

        return { width, height, channels, data };
    }

}