//
// Created by mecha on 22.10.2021.
//

#include "TextureSource.h"

namespace engine {

    void TextureSource::destroy() {
        _textures.clear();
    }

    bool TextureSource::exists(const std::string &name) {
        return _textures.find(name) != _textures.end();
    }

    const Ref<TextureBuffer>& TextureSource::getTextureBuffer(const std::string &name) {
        if (!exists(name)) {
            ENGINE_INFO("TextureBuffer {0} does not exists in cache! Creating it!", name);
            _textures[name] = _graphicsFactory->newTextureBuffer();
        }
        return _textures[name];
    }

    const uint32_t &TextureSource::loadTexture(const std::string &fileName) {
        return loadTexture(fileName, TEXTURE_PATH);
    }

    const uint32_t &TextureSource::loadTexture(const std::string &fileName, const std::string &texturesPath) {
        const auto& textureBuffer = getTextureBuffer(fileName);
        textureBuffer->setTexturesPath(texturesPath);
        textureBuffer->bind();
        textureBuffer->load(fileName);
        return textureBuffer->getId();
    }

}