//
// Created by mecha on 25.09.2021.
//

#include "TextureBuffer.h"

#include <sstream>
#include <stb_image.h>

namespace engine {

    void TextureBuffer::free() {
        stbi_image_free(textureData->data);
    }

    bool TextureBuffer::load(const std::string &imageName) {
        std::stringstream ss;
        ss << texturesPath << "/" << imageName;
        auto imagePath = ss.str();

        int width, height, channels;

        ENGINE_INFO("Loading texture from '{0}'", imagePath);
        auto data = stbi_load(imagePath.c_str(), &width, &height, &channels, 0);

        if (data == nullptr) {
            if (stbi_failure_reason()) {
                ENGINE_ERR("Error occurs when loading texture from '{0}'", imagePath);
                ENGINE_ERR("stbi failure reason : {0}", stbi_failure_reason());
                return false;
            }
        }

        textureData = new TextureData(width, height, channels, data);
        return true;
    }

}