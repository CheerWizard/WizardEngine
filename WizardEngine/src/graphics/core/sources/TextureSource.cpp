//
// Created by mecha on 22.10.2021.
//

#include "TextureSource.h"

namespace engine {

    Ref<TextureSource> TextureSource::instance = createRef<TextureSource>();

    bool TextureSource::exists(const std::string &name) {
        return _textures.find(name) != _textures.end();
    }

    const TextureBuffer& TextureSource::getTextureBuffer(const std::string &fileName) {
        return getTextureBuffer(fileName, RUNTIME_TEXTURES_PATH);
    }

    const uint32_t &TextureSource::loadTexture(const std::string &fileName) {
        return loadTexture(fileName, RUNTIME_TEXTURES_PATH);
    }

    const uint32_t &TextureSource::loadTexture(const std::string &fileName, const std::string &texturesPath) {
        // delete old texture and create/add new one!
        remove(fileName);
        return getTextureBuffer(fileName, texturesPath).getId();
    }

    const TextureBuffer& TextureSource::getTextureBuffer(
            const std::string &fileName,
            const std::string &texturesPath
    ) {
        if (!exists(fileName)) {
            ENGINE_INFO("TBO {0} does not exists in cache! Creating it! Textures path: {1}", fileName, texturesPath);
            auto newTextureBuffer = TextureBuffer();
            newTextureBuffer.bind();
            newTextureBuffer.loadFrom(fileName, texturesPath);
            newTextureBuffer.unbind();
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

    void TextureSource::clear() {
        for (auto& it : *this) {
            it.second.destroy();
        }
        _textures.clear();
    }

}