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

    // filePath should have ending with extension name , like .png or .jpg!
    bool TextureBuffer::load(const std::string &filePath) {
        setAssetName(filePath);
        int width, height, channels;

        ENGINE_INFO("Loading texture from '{0}'", CURRENT_WORKING_DIR + path);
        auto data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        // todo fix texture path not found.
        //  cannot open file from directory.
        //  possible to open if texture exists directly in WizardTest package
        //  tested also with '\' and '/' slash paths.

        if (data == nullptr) {
            if (stbi_failure_reason()) {
                ENGINE_ERR("Error occurs when loading texture from '{0}'", CURRENT_WORKING_DIR + path);
                ENGINE_ERR("stbi failure reason : {0}", stbi_failure_reason());
                return false;
            }
        }

        textureData = new TextureData(width, height, channels, data);
        return true;
    }

    const char* TextureBuffer::getExtensionName() const {
        return ""; // file can be either .png, .jpg, .jpeg!
    }

    const char* TextureBuffer::getAssetPath() const {
        return TEXTURE_PATHS;
    }

}