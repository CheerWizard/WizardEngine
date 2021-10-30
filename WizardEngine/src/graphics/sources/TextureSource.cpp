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

    const Ref<TextureBuffer>& TextureSource::getTextureBuffer(const std::string &fileName) {
        return getTextureBuffer(fileName, TEXTURE_PATH);
    }

    const uint32_t &TextureSource::loadTexture(const std::string &fileName) {
        return loadTexture(fileName, TEXTURE_PATH);
    }

    const uint32_t &TextureSource::loadTexture(const std::string &fileName, const std::string &texturesPath) {
        // delete old texture and create/add new one!
        remove(fileName);
        return getTextureBuffer(fileName, texturesPath)->getId();
    }

    const Ref<TextureBuffer>& TextureSource::getTextureBuffer(
            const std::string &fileName,
            const std::string &texturesPath
    ) {
        if (!exists(fileName)) {
            ENGINE_INFO("TBO {0} does not exists in cache! Creating it! Textures path: {1}", fileName, texturesPath);
            auto newTextureBuffer = _graphicsFactory->newTextureBuffer();
            newTextureBuffer->setTexturesPath(texturesPath);
            newTextureBuffer->bind();
            newTextureBuffer->load(fileName);
            newTextureBuffer->unbind();
            _textures[fileName] = newTextureBuffer;
        }
        return _textures[fileName];
    }

    void TextureSource::remove(const std::string &fileName) {
        if (!exists(fileName)) {
            ENGINE_INFO("TBO {0} can't be removed! Does not exists in cache!", fileName);
            return;
        }
        _textures.erase(fileName);
    }

}