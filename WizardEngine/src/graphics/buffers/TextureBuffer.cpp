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

    bool TextureBuffer::load(const std::string &filePath) {
//        std::stringstream srcPath;
//        srcPath << TEXTURE_PATHS << filePath;
        this->path = filePath;

        int width, height, channels;
//        stbi_set_flip_vertically_on_load(1);

        ENGINE_INFO("Loading texture from '{0}'", CURRENT_WORKING_DIR + path);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        // todo fix texture path not found.
        //  cannot open file from directory.
        //  possible to open if texture exists directly in WizardTest package
        // tested also with '\' and '/' slash paths.

        if (data == nullptr) {
            ENGINE_ERR("Error occurs when loading texture from '{0}'", CURRENT_WORKING_DIR + path);
            return false;
        }

        textureData = new TextureData(width, height, channels, data);
        return true;
    }

}