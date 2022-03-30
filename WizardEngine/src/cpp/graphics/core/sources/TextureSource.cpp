//
// Created by mecha on 22.10.2021.
//

#include <graphics/core/sources/TextureSource.h>

namespace engine {

    Ref<TextureSource> TextureSource::instance = createRef<TextureSource>();

    bool TextureSource::exists(const std::string &name) {
        return _textures.find(name) != _textures.end();
    }

    const TextureBuffer& TextureSource::getTextureBuffer(const TextureComponent& textureComponent) {
        return getTextureBuffer(textureComponent, RUNTIME_TEXTURES_PATH);
    }

    const TextureBuffer& TextureSource::getTextureBuffer(
            const TextureComponent& textureComponent,
            const std::string &texturesPath
    ) {
        return getTextureBuffer(textureComponent.type, textureComponent.fileName, texturesPath);
    }

    const TextureBuffer& TextureSource::getTextureBuffer(
            const TextureType &type,
            const std::string &fileName,
            const std::string &texturesPath
    ) {
        if (!exists(fileName)) {
            ENGINE_INFO("TBO {0} does not exists in cache! Creating it! Textures path: {1}", fileName, texturesPath);
            auto newTextureBuffer = TextureBuffer(type);
            newTextureBuffer.bind();
            newTextureBuffer.loadFrom(fileName, texturesPath);
            newTextureBuffer.unbind();
            _textures[fileName] = newTextureBuffer;
        }
        return _textures[fileName];
    }

    const TextureBuffer& TextureSource::getTextureBuffer(
            const CubeMapTextureComponent& cubeMapTextureComponent,
            const std::string& texturesPath
    ) {
        const auto& name = cubeMapTextureComponent.name;
        if (!exists(name)) {
            ENGINE_INFO("TBO {0} does not exists in cache! Creating it! Textures path: {1}", name, texturesPath);
            auto newTextureBuffer = TextureBuffer(TextureType::CUBE_MAP);
            newTextureBuffer.bind();
            newTextureBuffer.loadCubeMap(cubeMapTextureComponent.faces, texturesPath);
            newTextureBuffer.unbind();
            _textures[name] = newTextureBuffer;
        }
        return _textures[name];
    }

    uint32_t TextureSource::loadTexture(const TextureComponent& textureComponent) {
        return loadTexture(textureComponent, RUNTIME_TEXTURES_PATH);
    }

    uint32_t TextureSource::loadTexture(
            const TextureComponent& textureComponent,
            const std::string &texturesPath
    ) {
        return loadTexture(textureComponent.type, textureComponent.fileName, texturesPath);
    }

    uint32_t TextureSource::loadTexture(
            const TextureType &type,
            const std::string &fileName,
            const std::string &texturesPath
    ) {
        // delete old texture and create new one!
        remove(fileName);
        auto id = getTextureBuffer(type, fileName, texturesPath).getId();
        ENGINE_INFO("loadTexture(type = {0}, fileName = {1}, texturesPath = {2}) returns {3} id",
                    type, fileName, texturesPath, id);
        return id;
    }

    uint32_t TextureSource::loadTexture(
            const CubeMapTextureComponent& cubeMapTextureComponent,
            const std::string& texturesPath
    ) {
        // delete old texture and create new one!
        remove(cubeMapTextureComponent.name);
        return getTextureBuffer(cubeMapTextureComponent, texturesPath).getId();
    }

    void TextureSource::activate(const TextureComponent &textureComponent) {
        activate(textureComponent, RUNTIME_TEXTURES_PATH);
    }

    void TextureSource::activate(const TextureComponent &textureComponent, const std::string& assetPath) {
        auto& texture = getTextureBuffer(textureComponent, assetPath);
        texture.activate(textureComponent.sampler.value);
        texture.bind();
    }

    void TextureSource::activate(const CubeMapTextureComponent &cubeMapTextureComponent) {
        auto& texture = getTextureBuffer(cubeMapTextureComponent, RUNTIME_TEXTURES_PATH);
        texture.activate(cubeMapTextureComponent.sampler.value);
        texture.bind();
    }

    void TextureSource::remove(const std::string &fileName) {
        if (!exists(fileName)) {
            ENGINE_INFO("TBO {0} can't be removed! Does not exists in cache!", fileName);
            return;
        }
        _textures.erase(fileName);
    }

    void TextureSource::clear() {
        for (auto& tbo : _textures) {
            tbo.second.destroy();
        }
        _textures.clear();
    }
}
