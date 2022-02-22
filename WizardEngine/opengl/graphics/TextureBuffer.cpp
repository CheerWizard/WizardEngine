//
// Created by mecha on 25.09.2021.
//

#include <platform/graphics/TextureBuffer.h>
#include <glad/glad.h>

namespace engine {

    void TextureBuffer::create() {
        glCreateTextures(GL_TEXTURE_2D, 1, &id);
    }

    void TextureBuffer::destroy() {
        glDeleteTextures(1, &id);
    }

    void TextureBuffer::recreate() {
        destroy();
        create();
    }

    void TextureBuffer::bind() const {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void TextureBuffer::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void TextureBuffer::activate(const uint32_t &slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
    }

    void TextureBuffer::loadFrom(const std::string_view &fileName) {
        loadFrom(fileName, RUNTIME_TEXTURES_PATH);
    }

    void TextureBuffer::loadFrom(const std::string_view &fileName, const std::string_view &texturesPath) {
        auto textureData = TextureFile::read(fileName, texturesPath);
        if (textureData.data == nullptr) {
            ENGINE_WARN("Can't stream texture {0} from NULL data!", fileName);
        } else {
            load(textureData);
        }
    }

    void TextureBuffer::load(const TextureData &textureData) {
        GLenum internalFormat = 0, dataFormat = 0;
        int channels = textureData.channels, width = textureData.width, height = textureData.height;

        if (channels == 4) {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        } else if (channels == 3) {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        bool formatSupported = internalFormat & dataFormat;
        if (!formatSupported) {
            ENGINE_WARN(
                    "TextureBuffer {0} does not support format! internalFormat: {1}, dataFormat: {2}",
                    id,
                    internalFormat,
                    dataFormat
            );
        }

        glTextureStorage2D(id, 1, internalFormat, width, height);

        glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, textureData.data);

        TextureFile::free(textureData.data);
    }

}