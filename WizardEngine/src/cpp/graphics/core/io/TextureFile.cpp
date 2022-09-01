//
// Created by mecha on 10.11.2021.
//

#include <graphics/core/io/TextureFile.h>

#include <stb_image.h>
#include <sstream>

namespace engine::io {

    TextureData TextureFile::read(const char* filePath) {
        int width, height, channels;
        ENGINE_INFO("Reading texture from '{0}'", filePath);
        void* data = stbi_load(filePath, &width, &height, &channels, 0);
        if (data == nullptr) {
            if (stbi_failure_reason()) {
                ENGINE_ERR("Error occurs when loading texture from '{0}'", filePath);
                ENGINE_ERR("stbi failure reason : {0}", stbi_failure_reason());
            }
            return { width, height, channels };
        }

        return { width, height, channels, false, data };
    }

    void TextureFile::free(void *data) {
        stbi_image_free(data);
    }

}